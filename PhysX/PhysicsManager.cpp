#define __DONT_INCLUDE_MEM_LEAKS__

#include "Assert.h"
#include "PhysicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\BaseUtils.h"
#include "Utils\Defines.h"

////----PhysX Includes-------------
#undef min
#undef max
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"

#include "Actor\PhysicActor.h"
#include "Reports\PhysicCollisionReport.h"
#include "Actor\PhysicController.h"
#include "Cooking Mesh\PhysicCookingMesh.h"
#include "Joints\PhysicFixedJoint.h"
#include "Joints\PhysicRevoluteJoint.h"
#include "Joints\PhysicSphericalJoint.h"
#include "Reports\PhysicTriggerReport.h"
#include "Utils\PhysicUserAllocator.h"
#include "Utils\PhysicUserData.h"
#include "PhysicMaterial\PhysicMaterial.h"
////--------------------------------

#include "GraphicsManager.h"
#include "Utils\Exception.h"
#include "Utils\Logger.h"
#include "Utils\BaseUtils.h"

#if defined(_DEBUG)
//#include "Core_Utils\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CPhysicsManager::CPhysicsManager( void )
    : m_szConfigFileName	( "" )
    , m_bIsOk				( false )
    , m_bDebugRenderMode	( true )
    , m_pPhysicsSDK			( NULL )
    , m_pScene				( NULL )
    , m_pControllerManager	( NULL )
    , m_pMyAllocator		( NULL )
    , m_pCookingMesh		( NULL )
    , m_InitParams			( )
{
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CPhysicsManager::Init ( void )
{
    m_pMyAllocator = new CPhysicUserAllocator();
    assert(m_pMyAllocator);
    m_bIsOk = (m_pMyAllocator != NULL);
    if (m_bIsOk) {
        LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Inicializando la libreria PhysX");
        // Initialize PhysicsSDK
        NxPhysicsSDKDesc l_SDK_Desc;
        NxSDKCreateError errorCode = NXCE_NO_ERROR;
        m_pPhysicsSDK = NxCreatePhysicsSDK ( NX_PHYSICS_SDK_VERSION, m_pMyAllocator, NULL, l_SDK_Desc, &errorCode );
        //m_InitParams.m_fSkinWidth = 0.01f; //TODO: Borrar la línea
        m_bIsOk = (m_pPhysicsSDK != NULL);
        if (m_bIsOk) {
            LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager:: Creado el PhysXSDK" );
            LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager:: -------PhsX Settings---" );
            LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager:: El valor del SkinWidth es: %f", m_InitParams.m_fSkinWidth );
            m_pPhysicsSDK->setParameter( NX_SKIN_WIDTH, m_InitParams.m_fSkinWidth );
            //CODI PER PRINTAR INFO DELS JOINTS
            m_pPhysicsSDK->setParameter ( NX_VISUALIZE_ACTOR_AXES, 1 );
            m_pPhysicsSDK->setParameter ( NX_VISUALIZE_JOINT_LIMITS, 1 );
            m_pPhysicsSDK->setParameter ( NX_VISUALIZE_JOINT_LOCAL_AXES, 1 );
            m_pPhysicsSDK->setParameter ( NX_VISUALIZE_JOINT_WORLD_AXES, 1 );
            m_pPhysicsSDK->setParameter ( NX_CONTINUOUS_CD, 1 );
            // Create a scene
            LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> El valor de la gravedad es: %f", m_InitParams.m_fGravity);
            NxSceneDesc sceneDesc;
            sceneDesc.gravity = NxVec3(0.0f, m_InitParams.m_fGravity, 0.0f);
            sceneDesc.simType = NX_SIMULATION_HW;
            m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
            // Si no va per Harware busco per software
            if ( !m_pScene ) {
                sceneDesc.simType = NX_SIMULATION_SW;
                m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
            }
            m_bIsOk = (m_pScene != NULL);
            if (m_bIsOk) {
                LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> Solo hay un material, con los siguientes params" );
                LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->Restitution %f:", m_InitParams.m_Restitution_DefMat );
                LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->StaticFriction %f:", m_InitParams.m_StaticFriction_DefMat );
                LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->DynamicFriction %f:", m_InitParams.m_DynamicFriction_DefMat );
                LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> ----END PhsX Settings---" );
                // Set default material
                //TODO: Borrar líneas
                //m_InitParams.m_Restitution_DefMat = 0.5f;
                //m_InitParams.m_StaticFriction_DefMat = 0.5f;
                //m_InitParams.m_DynamicFriction_DefMat = 0.5f;
                NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
                defaultMaterial->setRestitution(m_InitParams.m_Restitution_DefMat);
                defaultMaterial->setStaticFriction(m_InitParams.m_StaticFriction_DefMat);
                defaultMaterial->setDynamicFriction(m_InitParams.m_DynamicFriction_DefMat);
                // Create a controllerManager
                m_pControllerManager = NxCreateControllerManager ( m_pMyAllocator );
                m_bIsOk = ( m_pControllerManager != NULL );
                if ( m_bIsOk ) {
                    LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> Creado el controlador de caracteres" );
                    m_pCookingMesh = new CPhysicCookingMesh();
                    assert ( m_pCookingMesh );
                    m_bIsOk = m_pCookingMesh->Init ( m_pPhysicsSDK, m_pMyAllocator );
                    if ( m_bIsOk ) {
                        LOGGER->AddNewLog ( ELL_INFORMATION, "PhysicsManager::Init-> Creado el CookingMesh" );
                    }
                }// isOk m_pControllerManager?
            }//isOk m_pScene?
        }//isOk m_pPhysicsSDK ?
    }//isOk m_pMyAllocator ?
    if ( !m_bIsOk ) {
        std::string msg_error = "PhysicsManager::Init-> Error en la inicializacion de PhysX";
        LOGGER->AddNewLog ( ELL_ERROR, msg_error.c_str() );
        Release();
        throw CException ( __FILE__, __LINE__, msg_error );
    }
    m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect("127.0.0.1");
    /*Precompilation Directives*/
#if defined( _DEBUG )
#define USE_DEBUGGER
#ifdef USE_DEBUGGER
    //m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect("127.0.0.1");
#endif
#endif
    return m_bIsOk;
}

void CPhysicsManager::SetGravity(Vect3f g)
{
    GetScene()->setGravity(NxVec3(g.x, g.y, g.z));
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicsManager::Done ()
{
    if (IsOk()) {
        Release();
        /* By XMA */
        ReleaseVectors();
        /**********/
        m_bIsOk = false;
    }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicsManager::Release ( void )
{
    ReleaseAllActors();
    CHECKED_DELETE ( m_pCookingMesh );
    if ( m_pControllerManager != NULL ) {
        m_pControllerManager->purgeControllers();
        NxReleaseControllerManager ( m_pControllerManager );
    }
    if ( m_pScene != NULL ) {
        m_pPhysicsSDK->releaseScene ( *m_pScene );
        m_pScene = NULL;
    }
    if ( m_pPhysicsSDK != NULL ) {
        NxReleasePhysicsSDK ( m_pPhysicsSDK );
        m_pPhysicsSDK = NULL;
    }
    CHECKED_DELETE ( m_pMyAllocator );
}

void CPhysicsManager::ReleaseVectors ()
{
    // UsersData & Actors del CreateMeshFromXML
    for (size_t i = 0; i < m_vUsersData.size(); ++i) {
        CHECKED_DELETE(m_vUsersData[i]);
    }
    m_vUsersData.clear();
    for (size_t i = 0; i < m_vActors.size(); ++i) {
        CHECKED_DELETE(m_vActors[i]);
    }
    m_vActors.clear();
}

void CPhysicsManager::ReleaseToReload ()
{
    GetCookingMesh()->Release();
    for (size_t i = 0; i < m_vActors.size(); ++i) {
        ReleasePhysicActor(m_vActors[i]);
    }
}

void CPhysicsManager::ReleaseElement ( const std::string &_ase )
{
    unsigned int l_id;
    std::map<std::string, unsigned int>::iterator it = m_vIds.find(_ase);
    if (it != m_vIds.end() ) {
        l_id = it->second;
    }
    GetCookingMesh()->ReleasePhysicMesh(_ase);
    ReleasePhysicActor(m_vActors[l_id]);
}


//----------------------------------------------------------------------------
// Load : Per carregar un fitxer XML amb tots els scrits
//----------------------------------------------------------------------------
bool CPhysicsManager::Load ( const std::string &_PhysXConfig )
{
    LOGGER->AddNewLog ( ELL_INFORMATION, "CPhysicsManager::Load-->Loading physics." );
    m_szConfigFileName = _PhysXConfig;
    Init();
    return LoadXML();
}

//----------------------------------------------------------------------------
// Reload : Per recarregar el XML
//----------------------------------------------------------------------------
bool CPhysicsManager::Reload ( void )
{
    LOGGER->AddNewLog ( ELL_INFORMATION, "CPhysicsManager::Reload-> Reloading physics.");
    /* By XMA */
    ReleaseToReload();
    ReleaseVectors();
    Init();
    return CreateMeshFromXML(m_FileName);
}

bool CPhysicsManager::ReloadXML ( const std::string &FileName )
{
    LOGGER->AddNewLog ( ELL_INFORMATION, "CPhysicsManager::Reload-> Reloading physics.");
    Release ();
//	return LoadXML ();
    /* By XMA */
    Init();
    return CreateMeshFromXML(FileName);
}

//----------------------------------------------------------------------------
// LoadXML : Carrega realment el XML
//----------------------------------------------------------------------------
bool CPhysicsManager::LoadXML ( void )
{
    LOGGER->AddNewLog ( ELL_INFORMATION, "CPhysicsManager::LoadXML --> Loading physic Files..." );
    /*CXMLTreeNode l_File;

    if ( l_File.LoadFile ( m_FileName.c_str () ) )
    {
    	CXMLTreeNode l_Scripts = l_File["scripts"];
    	if ( l_Scripts.Exists ( ) )
    	{
    		unsigned int l_NumChilds = l_Scripts.GetNumChildren ( );
    		for ( unsigned int i = 0; i < l_NumChilds; i ++ )
    		{
    			std::string l_Type = l_Scripts(i).GetName();
    			if ( l_Type == "script" )
    			{
    				std::string l_FileName = l_Scripts(i).GetPszProperty ( "filename" );		// Obtenemos la ruta del fichero lua
    				RunFile ( l_FileName );														// Ejecutamos el fichero lua
    			}
    			else
    			{
    				LOGGER->AddNewLog ( ELL_WARNING, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't contain any tag different form <SCRIPT>.", m_FileName );
    			}
    		}
    	}
    	else
    	{
    		LOGGER->AddNewLog ( ELL_ERROR, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't contain tag <SCRIPTS>.", m_FileName );
    		return false;
    	}
    }
    else
    {
    	LOGGER->AddNewLog ( ELL_ERROR, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't exist or contain sintaxis errors.", m_FileName );
    	return false;
    }
    */
    return true;
}


// -----------------------------------------
//					MÈTODES
// -----------------------------------------

//----------------------------------------------------------------------------
// Update : Per actualitzar l'escena i realitzar les físiques i simulacions
//----------------------------------------------------------------------------
void CPhysicsManager::Update ( float _ElapsedTime )
{
    assert( m_pScene != NULL );
    assert( m_pControllerManager != NULL );
    // Start simulation (non blocking)
    m_pScene->simulate ( _ElapsedTime );
    // Fetch simulation results
    m_pScene->flushStream ( );
    m_pScene->fetchResults ( NX_RIGID_BODY_FINISHED,  true );
}

void CPhysicsManager::WaitForSimulation ( void )
{
    m_pScene->fetchResults ( NX_RIGID_BODY_FINISHED,  true );
    NxReal l_MaxTimestep;
    NxTimeStepMethod l_Method;
    NxU32 l_MaxIter;
    NxU32 l_NumSubSteps;
    m_pScene->getTiming( l_MaxTimestep, l_MaxIter, l_Method, &l_NumSubSteps);
    if ( l_NumSubSteps ) {
        m_pControllerManager->updateControllers();
    }
}

//----------------------------------------------------------------------------------------
// Debug Render : dibuixa cada un dels actors trobats a l'escena si estem en mode debug
//----------------------------------------------------------------------------------------
void CPhysicsManager::DebugRender ( CGraphicsManager* _RM )
{
    assert( m_pScene != NULL );
    if ( !m_bDebugRenderMode ) return;
    // Render all actors in the scene
    int nbActors = m_pScene->getNbActors();
    NxActor** l_pActors = m_pScene->getActors();
    while ( nbActors-- ) {
        NxActor* l_pActor = *l_pActors++;
        DrawActor ( l_pActor, _RM );
    }
}

//----------------------------------------------------------------------------
// DrawActor : Dibuixa l'actor en mode debug
//----------------------------------------------------------------------------
void CPhysicsManager::DrawActor ( NxActor* _pActor, CGraphicsManager* _RM )
{
    CPhysicUserData* physicUserData = NULL;
    physicUserData = ( CPhysicUserData* ) _pActor->userData;
    //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
    assert(physicUserData);
    if ( !physicUserData->GetPaint()) {
        return;
    }
    NxShape*const* shapes = _pActor->getShapes();
    NxU32 nShapes = _pActor->getNbShapes();
    nShapes = _pActor->getNbShapes();
    while (nShapes--) {
        switch (shapes[nShapes]->getType()) {
        case NX_SHAPE_PLANE: {
                CColor color = colBLACK;
                float distance = shapes[nShapes]->isPlane()->getPlane().d;
                NxVec3 normal =  shapes[nShapes]->isPlane()->getPlane().normal;
                Vect3f n(normal.x, normal.y, normal.z);
                _RM->DrawPlane(100.f, n, distance, color, 40, 40);
            }
            break;
        case NX_SHAPE_BOX: {
                NxVec3 pos = shapes[nShapes]->getGlobalPosition();
                NxF32 m_aux[16];
                shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
                Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12],
                            m_aux[1], m_aux[5], m_aux[9], m_aux[13],
                            m_aux[2], m_aux[6], m_aux[10], m_aux[14],
                            m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
                _RM->SetTransform(m);
                NxVec3 boxDim = shapes[nShapes]->isBox()->getDimensions();
                CColor color = physicUserData->GetColor();
                //CColor	color = colRED;
                _RM->DrawBox(2 * boxDim.x, 2 * boxDim.y, 2 * boxDim.z, color);
                //_RM->DrawCube(boxDim.y*2,color);
                _RM->SetTransform(m44fIDENTITY.GetD3DXMatrix());
            }
            break;
        case NX_SHAPE_SPHERE: {
                NxF32 m_aux[16];
                shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
                Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12],
                            m_aux[1], m_aux[5], m_aux[9], m_aux[13],
                            m_aux[2], m_aux[6], m_aux[10], m_aux[14],
                            m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
                _RM->SetTransform(m);
                NxReal radius = shapes[nShapes]->isSphere()->getRadius();
                CColor color = physicUserData->GetColor();
                _RM->DrawSphere(radius,  color, MAX_ARISTAS);
            }
            break;
        case NX_SHAPE_CAPSULE: {
                NxF32 m_aux[16];
                shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
                Mat44f m( m_aux[0], m_aux[4], m_aux[8], m_aux[12],
                          m_aux[1], m_aux[5], m_aux[9], m_aux[13],
                          m_aux[2], m_aux[6], m_aux[10], m_aux[14],
                          m_aux[3], m_aux[7], m_aux[11], m_aux[15] );
                Mat44f translation, total;
                translation.SetIdentity();
                _RM->SetTransform(m);
                const NxReal & radius = shapes[nShapes]->isCapsule()->getRadius();
                const NxReal & height = shapes[nShapes]->isCapsule()->getHeight();
                CColor color = physicUserData->GetColor();
                translation.Translate(Vect3f(0.f, (height * 0.5f), 0.f));
                total = m * translation;
                _RM->SetTransform(total);
                _RM->DrawSphere(radius, color, MAX_ARISTAS);
//				_RM->DrawHalfSupSphere(radius, MAX_ARISTAS, color); // By XMA
                translation.Translate( Vect3f(0.f, -(height * 0.5f), 0.f ));
                total = m * translation;
                _RM->SetTransform(total);
                _RM->DrawSphere(radius, color, MAX_ARISTAS);
//				_RM->DrawHalfInfSphere(radius, MAX_ARISTAS, color); // By XMA
                /* By XMA */
                for (float h = -(height * 0.5f); h <= (height * 0.5f); h += (height * 0.125f)) {
                    translation.Translate(Vect3f(0.f, h, 0.f));
                    total = m * translation;
                    _RM->SetTransform(total);
//					if(h < 0.f) _RM->DrawCylinderCircs2(radius, MAX_ARISTAS, color);
//					else _RM->DrawCylinderCircs1(radius, MAX_ARISTAS, color);
                }
//				_RM->DrawCylinderLines(radius, MAX_ARISTAS, -height, color); // By XMA
            }
            break;
        case NX_SHAPE_CONVEX:
            break;
        case NX_SHAPE_MESH: {
                NxShape* mesh = shapes[nShapes];
                NxTriangleMeshDesc meshDesc;
                mesh->isTriangleMesh()->getTriangleMesh().saveToDesc(meshDesc);
                typedef NxVec3 Point;
                typedef struct _Triangle {
                    NxU32 p0;
                    NxU32 p1;
                    NxU32 p2;
                } Triangle;
                NxU32 nbVerts = meshDesc.numVertices;
                NxU32 nbTriangles = meshDesc.numTriangles;
                Point* points = (Point *)meshDesc.points;
                Triangle* triangles = (Triangle *)meshDesc.triangles;
                CColor color = physicUserData->GetColor();
                NxF32 m_aux[16];
                mesh->getGlobalPose().getColumnMajor44(m_aux);
                Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12],
                            m_aux[1], m_aux[5], m_aux[9], m_aux[13],
                            m_aux[2], m_aux[6], m_aux[10], m_aux[14],
                            m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
                _RM->SetTransform(m);
                Vect3f a, b, c;
                while (nbTriangles--) {
                    a = Vect3f(points[triangles->p0].x, points[triangles->p0].y, points[triangles->p0].z);
                    b = Vect3f(points[triangles->p1].x, points[triangles->p1].y, points[triangles->p1].z);
                    c = Vect3f(points[triangles->p2].x, points[triangles->p2].y, points[triangles->p2].z);
                    _RM->DrawLine(a, b, color);
                    _RM->DrawLine(b, c, color);
                    _RM->DrawLine(c, a, color);
                    triangles++;
                }
            }
            break;
        case NX_SHAPE_WHEEL: {
                //TODO...
            }
            break;
        default: {
                //TODO...
            }
            break;
        }
    }
}

//----------------------------------------------------------------------------
// AddPhysicActor : Afegeix un actor a l'escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicActor ( CPhysicActor* _pActor )
{
    assert ( _pActor != NULL );
    assert ( m_pScene != NULL );
    bool l_bIsOK = false;
    NxActor* nxActor;
    NxActorDesc* l_pActorDesc = _pActor->GetActorDesc();
    assert ( l_pActorDesc != NULL );
    nxActor = m_pScene->createActor( *l_pActorDesc );
    if ( nxActor != NULL ) {
        nxActor->userData = _pActor->GetUserData();
        _pActor->CreateActor ( nxActor );
        l_bIsOK = true;
    }
    if ( l_bIsOK ) {
        _pActor->SetCollisionGroup( _pActor->GetColisionGroup() );
    }
    return l_bIsOK;
}

//----------------------------------------------------------------------------
// ReleasePhysicActor : Alliberem un actor de l'escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleasePhysicActor ( CPhysicActor* _pActor )
{
    assert ( _pActor != NULL );
    assert ( m_pScene != NULL );
    assert ( m_pPhysicsSDK != NULL );
    bool isOk = false;
    NxActor* nxActor = _pActor->GetPhXActor();
    if ( nxActor != 0) {
        NxArray<NxCCDSkeleton*> skeletons;
        for (NxU32 i = 0; i < nxActor->getNbShapes(); i++) {
            NxShape* shape = nxActor->getShapes()[i];
            if (shape->getCCDSkeleton() != NULL) {
                skeletons.pushBack(shape->getCCDSkeleton());
            }
        }
        for (NxU32 i = 0; i < skeletons.size(); i++) {
            m_pPhysicsSDK->releaseCCDSkeleton(*skeletons[i]);
        }
        m_pScene->releaseActor(*nxActor);
        nxActor = 0;
    }
    //TODO!!!!
    return true;
}

//----------------------------------------------------------------------------
// ReleaseAllActors : Alliberem tots els actors de l'escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleaseAllActors ( void ) //EUserDataFlag _eFlags )
{
    assert ( m_pScene != NULL );
    assert ( m_pPhysicsSDK != NULL );
    bool isOk = true;
    NxActor** l_ppActorList = m_pScene->getActors();
    NxU32 l_TotalActors		= m_pScene->getNbActors();
    while ( l_TotalActors -- ) {
        NxActor* nxActor = *l_ppActorList;
        if ( nxActor != 0) {
            NxArray<NxCCDSkeleton*> skeletons;
            for (NxU32 i = 0; i < nxActor->getNbShapes(); i++) {
                NxShape* shape = nxActor->getShapes()[i];
                if (shape->getCCDSkeleton() != NULL) {
                    skeletons.pushBack(shape->getCCDSkeleton());
                }
            }
            for (NxU32 i = 0; i < skeletons.size(); i++) {
                m_pPhysicsSDK->releaseCCDSkeleton(*skeletons[i]);
            }
            m_pScene->releaseActor(*nxActor);
            nxActor = 0;
        }
    }
    return isOk;
}

//----------------------------------------------------------------------------
// AddMaterial : Afegim un material a l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddMaterial(CPhysicMaterial* _pMaterial)
{
    NxMaterial* mat;
    NxMaterialDesc* des = _pMaterial->getMaterialDesc();
    mat = m_pScene->createMaterial(*des);
    _pMaterial->setMaterial(mat);
    return true;
}

//----------------------------------------------------------------------------
// AddMaterial : Afegim un material a l'escena
//----------------------------------------------------------------------------

bool CPhysicsManager::ReleaseMaterial (CPhysicMaterial* _pMaterial)
{
    m_pScene->releaseMaterial(*(_pMaterial->getMaterial()));
    return true;
}

//----------------------------------------------------------------------------
// AddPhysicSphericalJoint : Afegim un joint esféric a l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicSphericalJoint ( CPhysicSphericalJoint* _Joint )
{
    assert ( _Joint != NULL );
    assert ( m_pScene != NULL );
    bool l_IsOk = false;
    NxJoint* l_NxJoint;
    NxSphericalJointDesc* l_JointDesc = _Joint->GetPhXDescJoint();
    assert ( l_JointDesc != NULL );
    l_NxJoint = m_pScene->createJoint ( *l_JointDesc );
    if ( l_NxJoint != NULL ) {
        _Joint->CreateJoint( l_NxJoint );
        l_IsOk = true;
    }
    return l_IsOk;
}

//----------------------------------------------------------------------------
// RelasePhysicSphericalJoint : Alliberem un joint esféric de l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::RelasePhysicSphericalJoint ( CPhysicSphericalJoint* _Joint )
{
    assert ( _Joint );
    assert ( m_pScene );
    NxJoint* l_PhXJoint = _Joint->GetPhXJoint();
    m_pScene->releaseJoint ( *l_PhXJoint );
    l_PhXJoint = 0;
    return true;
}

//----------------------------------------------------------------------------
// AddPhysicRevoluteJoint : Afegim un joint de tipus visagre de porta a l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicRevoluteJoint ( CPhysicRevoluteJoint* _pJoint )
{
    assert ( _pJoint != NULL );
    assert ( m_pScene != NULL );
    bool l_IsOk = false;
    NxJoint* nxJoint = 0;
    NxRevoluteJointDesc* l_JointDesc = _pJoint->GetPhXDescJoint();
    assert ( l_JointDesc != NULL );
    nxJoint = m_pScene->createJoint ( *l_JointDesc );
    if ( nxJoint != NULL ) {
        _pJoint->CreateJoint ( nxJoint );
        l_IsOk = true;
    }
    return l_IsOk;
}

//----------------------------------------------------------------------------
// RelasePhysicRevoluteJoint : Alliberem un joint de bisagre de l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::RelasePhysicRevoluteJoint ( CPhysicRevoluteJoint* _Joint)
{
    assert ( _Joint );
    assert ( m_pScene );
    NxJoint* l_PhXJoint = _Joint->GetPhXJoint();
    m_pScene->releaseJoint(*l_PhXJoint);
    l_PhXJoint = 0;
    return true;
}

//----------------------------------------------------------------------------
// AddPhysicFixedJoint : Afegim un joint de premsa, com un amortiguador a l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicFixedJoint ( CPhysicFixedJoint* _pJoint )
{
    assert ( _pJoint != NULL );
    assert ( m_pScene != NULL );
    bool l_bIsOk = false;
    NxJoint* l_NxJoint = 0;
    NxFixedJointDesc* l_pJointDesc = _pJoint->GetPhXDescJoint();
    assert ( l_pJointDesc != NULL );
    l_NxJoint = m_pScene->createJoint(*l_pJointDesc);
    if ( l_NxJoint != NULL ) {
        _pJoint->CreateJoint ( l_NxJoint );
        l_bIsOk = true;
    }
    return l_bIsOk;
}

//----------------------------------------------------------------------------
// RelasePhysicFixedJoint : Alliberem un joint fixe de l'escena
//----------------------------------------------------------------------------
bool CPhysicsManager::RelasePhysicFixedJoint ( CPhysicFixedJoint* _pJoint )
{
    assert ( _pJoint );
    assert ( m_pScene );
    NxJoint* l_PhXJoint = _pJoint->GetPhXJoint();
    m_pScene->releaseJoint(*l_PhXJoint);
    l_PhXJoint = 0;
    return true;
}

bool CPhysicsManager::AddPhysicController ( CPhysicController* _pController, EControleType _Tipus, ECollisionGroup _Group )
{
    assert ( _pController != NULL );
    assert ( m_pScene != NULL );
    assert ( m_pControllerManager != NULL );
    bool l_bIsOK = false;
    NxController* l_NxController = _pController->GetPhXController();
    assert ( l_NxController == NULL ); //Nos aseguramos que no hayan registrado ya un actor en la escena
    switch ( _pController->GetType() ) {
    case BOX: {
            NxControllerDesc* l_NxControllerDesc = NULL;
            l_NxControllerDesc = _pController->GetPhXControllerDesc();
            assert ( l_NxControllerDesc != NULL );
            l_NxController = m_pControllerManager->createController ( m_pScene, *l_NxControllerDesc );
            break;
        }
    case CAPSULE: {
            NxControllerDesc* l_NxControllerDesc = NULL;
            l_NxControllerDesc = _pController->GetPhXControllerDesc();
            assert ( l_NxControllerDesc != NULL );
            l_NxController = m_pControllerManager->createController ( m_pScene, *l_NxControllerDesc );
            break;
        }
    }
    if ( m_pControllerManager != NULL ) {
        _pController->CreateController ( l_NxController, m_pScene );
        l_NxController->getActor()->userData = _pController->GetUserData();
        //NxShape*const* shape = nxController->getActor()->getShapes();
        //shape[0]->setGroup(controller->);
        l_NxController->getActor()->getShapes()[0]->setGroup(_Group);
        l_bIsOK = true;
    }
    if ( l_bIsOK )
        _pController->SetGroup( _pController->GetColisionGroup() );
    return l_bIsOK;
}

bool CPhysicsManager::ReleasePhysicController ( CPhysicController* _pController )
{
    assert ( _pController != NULL );
    assert ( m_pControllerManager != NULL );
    bool l_bIsOk = false;
    NxController* l_NxController = _pController->GetPhXController();
    if ( l_NxController != NULL ) {
        m_pControllerManager->releaseController( *l_NxController );
    }
    //TODO!!!!
    return true;
}

NxCCDSkeleton* CPhysicsManager::CreateCCDSkeleton (float size)
{
    assert( m_pPhysicsSDK != NULL );
    NxU32 triangles[3 * 12] = { 0, 1, 3, 	0, 3, 2, 3, 7, 6, 3, 6, 2, 1, 5, 7, 1, 7, 3, 4, 6, 7, 4, 7, 5, 1, 0, 4,
                                5, 1, 4,	4, 0, 2,	4, 2, 6
                              };
    NxVec3 points[8];
    //static mesh
    points[0].set( size, -size, -size);
    points[1].set( size, -size,  size);
    points[2].set( size,  size, -size);
    points[3].set( size,  size,  size);
    points[4].set(-size, -size, -size);
    points[5].set(-size, -size,  size);
    points[6].set(-size,  size, -size);
    points[7].set(-size,  size,  size);
    NxSimpleTriangleMesh stm;
    stm.numVertices = 8;
    stm.numTriangles = 6 * 2;
    stm.pointStrideBytes = sizeof(NxVec3);
    stm.triangleStrideBytes = sizeof(NxU32) * 3;
    stm.points = points;
    stm.triangles = triangles;
    stm.flags |= NX_MF_FLIPNORMALS;
    return m_pPhysicsSDK->createCCDSkeleton(stm);
}

//CPhysicUserData* CPhysicsManager::RaycastClosestActor ( const Vect3f _vPosRay, const Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info )
//{
//  //NxUserRaycastReport::ALL_SHAPES
//	assert(m_pScene != NULL);
//
//	NxRay ray;
//	ray.dir =  NxVec3 ( _vDirRay.x, _vDirRay.y, _vDirRay.z );
//	ray.orig = NxVec3 ( _vPosRay.x, _vPosRay.y, _vPosRay.z );
//
//	NxRaycastHit hit;
//	NxShape* closestShape = NULL;
//
//	//closestShape = m_pScene->raycastClosestShape ( ray, NX_ALL_SHAPES, hit, _uiImpactMask,  NX_MAX_F32, _uiImpactMask );
//	//closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, 0xffffffff, NX_MAX_F32, 0xffffffff, NULL, NULL );
//	closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, _uiImpactMask );
//	if (!closestShape)
//	{
//		//No hemos tocado a ningún objeto físico de la escena.
//		return NULL;
//	}
//	NxActor* actor = &closestShape->getActor();
//	CPhysicUserData* impactObject =(CPhysicUserData*)actor->userData;
//	//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
//	assert(impactObject);
//
//	_Info.m_fDistance		= hit.distance;
//	_Info.m_Normal			= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
//	_Info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
//
//	return impactObject;
//}

CPhysicUserData* CPhysicsManager::RaycastClosestActor( const Vect3f _vPosRay, const Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info, float _uiMaxDistance )
{
    //NxUserRaycastReport::ALL_SHAPES
    assert(m_pScene != NULL);
    NxRay ray;
    ray.dir =  NxVec3 ( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    ray.orig = NxVec3 ( _vPosRay.x, _vPosRay.y, _vPosRay.z );
    NxRaycastHit hit;
    NxShape* closestShape = NULL;
    //closestShape = m_pScene->raycastClosestShape ( ray, NX_ALL_SHAPES, hit, _uiImpactMask,  NX_MAX_F32, _uiImpactMask );
    //closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, 0xffffffff, NX_MAX_F32, 0xffffffff, NULL, NULL );
    NxReal l_Distance = (NxReal) _uiMaxDistance;
    // --- Jordi : Provisional. Cal deixar aquesta linia i modificar la col·lisió de càmera
    closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, _uiImpactMask, l_Distance );
    if (!closestShape) {
        //No hemos tocado a ningún objeto físico de la escena.
        return NULL;
    }
    NxActor* actor = &closestShape->getActor();
    CPhysicUserData* impactObject = (CPhysicUserData*)actor->userData;
    //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
    assert(impactObject);
    _Info.m_fDistance		= hit.distance;
    _Info.m_Normal			= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
    _Info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
    return impactObject;
}

CPhysicUserData* CPhysicsManager::RaycastClosestActorShoot ( const Vect3f _vPosRay, const Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info, float _fPower )
{
    //NxUserRaycastReport::ALL_SHAPES
    assert(m_pScene != NULL);
    NxRay ray;
    ray.dir =  NxVec3 ( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    ray.orig = NxVec3 ( _vPosRay.x, _vPosRay.y, _vPosRay.z );
    NxRaycastHit hit;
    NxShape* closestShape = NULL;
    closestShape = m_pScene->raycastClosestShape ( ray, NX_ALL_SHAPES, hit, _uiImpactMask );
    if (!closestShape) {
        //No hemos tocado a ningún objeto físico de la escena.
        return NULL;
    }
    NxActor* actor = &closestShape->getActor();
    CPhysicUserData* impactObject = (CPhysicUserData*)actor->userData;
    //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
    assert(impactObject);
    _Info.m_fDistance		= hit.distance;
    _Info.m_Normal			= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
    _Info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
    Vect3f l_vDirection( _vDirRay.x - _vPosRay.x, _vDirRay.y - _vPosRay.y, _vDirRay.z - _vPosRay.z );
    l_vDirection.Normalize();
    NxVec3 l_vDirectionVec( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    NxF32 coeff = actor->getMass() * _fPower;
    actor->addForceAtLocalPos ( l_vDirectionVec * coeff, NxVec3(0, 0, 0), NX_IMPULSE, true );
    return impactObject;
}

void CPhysicsManager::OverlapSphereActor ( float _fRadiusSphere, const Vect3f& _vPosSphere, std::vector<CPhysicUserData*> &_ImpactObjects, uint32 _uiImpactMask )
{
    assert ( m_pScene );
    NxSphere l_WorldSphere ( NxVec3 ( _vPosSphere.x, _vPosSphere.y, _vPosSphere.z ), _fRadiusSphere );
    NxU32 nbShapes = m_pScene->getNbDynamicShapes();
    NxShape** shapes = new NxShape* [nbShapes];
    for ( NxU32 i = 0; i < nbShapes; i++ ) {
        shapes[i] = NULL;
    }
    m_pScene->overlapSphereShapes ( l_WorldSphere, NX_ALL_SHAPES, nbShapes, shapes, NULL, _uiImpactMask );
    for (NxU32 i = 0; i < nbShapes; i++) {
        if ( shapes[i] != NULL ) {
            NxActor* l_pActor = &shapes[i]->getActor();
            CPhysicUserData* l_pPhysicObject = (CPhysicUserData*) l_pActor->userData;
            //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
            assert ( l_pPhysicObject );
            //Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
            std::vector<CPhysicUserData*>::iterator it ( _ImpactObjects.begin() );
            std::vector<CPhysicUserData*>::iterator itEnd ( _ImpactObjects.end() );
            bool find = false;
            while ( it != itEnd ) {
                CPhysicUserData* id = *it;
                if ( id == l_pPhysicObject )
                    find = true;
                ++it;
            }
            if (!find)
                _ImpactObjects.push_back ( l_pPhysicObject );
        }
    }
    delete shapes;
}

void CPhysicsManager::OverlapSphereActorGrenade (float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower)
{
    assert(m_pScene);
    NxSphere worldSphere(NxVec3(posSphere.x, posSphere.y, posSphere.z), radiusSphere);
    NxU32 nbShapes = m_pScene->getNbDynamicShapes();
    NxShape** shapes = new NxShape* [nbShapes];
    for (NxU32 i = 0; i < nbShapes; i++) {
        shapes[i] = NULL;
    }
    //NX_DYNAMIC_SHAPES
    m_pScene->overlapSphereShapes(worldSphere, NX_DYNAMIC_SHAPES, nbShapes, shapes, NULL);
    for (NxU32 i = 0; i < nbShapes; i++) {
        if ( shapes[i] != NULL ) {
            NxActor* actor = &shapes[i]->getActor();
            CPhysicUserData* physicObject = (CPhysicUserData*)actor->userData;
            //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
            assert(physicObject);
            //Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
            std::vector<CPhysicUserData*>::iterator it(impactObjects.begin());
            std::vector<CPhysicUserData*>::iterator itEnd(impactObjects.end());
            bool find = false;
            while (it != itEnd) {
                CPhysicUserData* id = *it;
                if ( id == physicObject)
                    find = true;
                ++it;
            }
            if (!find) {
                impactObjects.push_back(physicObject);
                physicObject->SetColor(colRED);
                ApplyExplosion(actor, posSphere, radiusSphere, _fPower);
            }
        }
        //delete &shapes[i];
    }
    delete shapes;
    /*for (NxU32 i = 0; i < nbShapes; i++)
    {
    delete &shapes[i];
    }*/
}

////void CPhysicsManager::RegisterFunctions (CScriptManager* scriptManager)
////{
////	//lua_State* l_pLUAState = scriptManager->GetLuaState();
////
////	//using namespace luabind;
////	//
////	//// ahora registramos lo que querramos
////	// module(l_pLUAState)
////	//   [
////	//     // registramos la clase CPhysicsManager
////	//		class_<CPhysicsManager>(CScriptRegister::SetClassName("CPhysicsManager"))
////
////
////	//     // registramos su constructor
////	//     .def(constructor<>())
////
////	//     // registramos sus funciones publicas
////	//		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)
////
////	//		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
////	//					"Muestra todas las funciones de esta clase"),
////	//					&CScriptRegister::Help)
////
////	//     .def(	CScriptRegister::PushFunctionName("setDebugRender", "void", "bool flag",
////	//					"Setea a true/false la visibilidad de todos los objetos físicos de la escena"),
////	//					&CPhysicsManager::SetDebugRenderMode)
////
////	//		.def(	CScriptRegister::PushFunctionName("getDebugRender", "bool", "void",
////	//					"Obtiene si se visualiza o no los objetos físicos de la escena"),
////	//					&CPhysicsManager::GetDebugRenderMode)
////	//   ];
////}

void CPhysicsManager::ApplyExplosion(NxActor* _pActor, const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower)
{
    Vect3f l_vVelocityDirection;
    Vect3f l_vActorPosition;
    float l_fDistance;
    float l_fTotalPower;
    NxVec3 l_vPos = _pActor->getGlobalPosition();
    l_vActorPosition = Vect3f(l_vPos.x, l_vPos.y, l_vPos.z);
    l_vVelocityDirection = l_vActorPosition - _vPosSphere;
    l_vVelocityDirection.Normalize();
    l_fDistance = _vPosSphere.Distance(l_vActorPosition);
    l_fTotalPower = _fPower * ((_fEffectRadius - l_fDistance) / _fEffectRadius);
    NxF32 coeff = _pActor->getMass() * l_fTotalPower;
    NxVec3 l_vDirection(l_vVelocityDirection.x, l_vVelocityDirection.y, l_vVelocityDirection.z);
    _pActor->addForceAtLocalPos(l_vDirection * coeff, NxVec3(0, 0, 0), NX_IMPULSE, true);
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------

void CPhysicsManager::SetTriggerReport ( CPhysicTriggerReport* _Report )
{
    assert ( m_pScene );
    assert ( _Report );
    NxUserTriggerReport* nxUserTriggerReport = (NxUserTriggerReport*) _Report;
    m_pScene->setUserTriggerReport ( nxUserTriggerReport );
}

void CPhysicsManager::SetCollisionReport ( CPhysicCollisionReport* _Report )
{
    assert ( m_pScene );
    assert ( _Report );
    NxUserContactReport* nxContactReport = (NxUserContactReport*) _Report;
    m_pScene->setUserContactReport ( nxContactReport );
}


// TODO!!
CPhysicActor* CPhysicsManager::GetActor ( std::string _ActorName )
{
    assert ( m_pScene );
    //NxActor** l_ppActorList = m_pScene->getActors();
    //NxU32 l_TotalActors	  = m_pScene->getNbActors();
    //
    CPhysicActor * l_pActor = NULL;
    int nbActors = m_pScene->getNbActors();
    NxActor** l_pActors = m_pScene->getActors();
    while ( nbActors-- ) {
        NxActor* l_pPhysicActor = *l_pActors++;
        CPhysicUserData* l_pPhysicUserData = NULL;
        l_pPhysicUserData = ( CPhysicUserData* ) l_pPhysicActor->userData;
        //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
        assert(l_pPhysicUserData);
        if ( l_pPhysicUserData->getName() == _ActorName ) {
            //	l_pActor = new CPhysicActor(l_pPhysicActor);
            l_pActor = l_pPhysicUserData->GetActor(); // Linea de código añadida por XMA
            break;
        }
    }
    return l_pActor;
}
void CPhysicsManager::SetPaintAllActors(bool paint, std::string meshName, bool paintMesh)
{
    std::vector<CPhysicActor*> l_Actors;
    assert ( m_pScene );
    //NxActor** l_ppActorList = m_pScene->getActors();
    //NxU32 l_TotalActors	  = m_pScene->getNbActors();
    //
    CPhysicActor * l_pActor = NULL;
    int nbActors = m_pScene->getNbActors();
    NxActor** l_pActors = m_pScene->getActors();
    while ( nbActors-- ) {
        NxActor* l_pPhysicActor = *l_pActors++;
        CPhysicUserData* l_pPhysicUserData = NULL;
        l_pPhysicUserData = ( CPhysicUserData* ) l_pPhysicActor->userData;
        //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
        assert(l_pPhysicUserData);
        // if ( l_pPhysicUserData->getName() == _ActorName ) {
        //	l_pActor = new CPhysicActor(l_pPhysicActor);
        if (l_pPhysicUserData->getName() != meshName)
            l_pPhysicUserData->SetPaint(paint); // Linea de código añadida por XMA
        else
            l_pPhysicUserData->SetPaint(paintMesh);
        //   break;
        // }
    }
}

void CPhysicsManager::SetPaintByActorName(std::string name, bool paint)
{
    assert ( m_pScene );
    //NxActor** l_ppActorList = m_pScene->getActors();
    //NxU32 l_TotalActors	  = m_pScene->getNbActors();
    //
    CPhysicActor * l_pActor = NULL;
    int nbActors = m_pScene->getNbActors();
    NxActor** l_pActors = m_pScene->getActors();
    while ( nbActors-- ) {
        NxActor* l_pPhysicActor = *l_pActors++;
        CPhysicUserData* l_pPhysicUserData = NULL;
        l_pPhysicUserData = ( CPhysicUserData* ) l_pPhysicActor->userData;
        //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
        assert(l_pPhysicUserData);
        if ( l_pPhysicUserData->getName() == name ) {
            l_pPhysicUserData->SetPaint(paint);
            break;
        }
    }
}


int CPhysicsManager::GetCollisionGroup( const std::string& _szGroup )
{
    if (_szGroup.compare("ECG_ESCENE") == 0) {
        return ECG_ESCENE;
    } else if (_szGroup.compare("ECG_PLAYER") == 0) {
        return ECG_PLAYER;
    } else if (_szGroup.compare("ECG_ENEMY") == 0) {
        return ECG_ENEMY;
    } else if (_szGroup.compare("ECG_TRIGGERS") == 0) {
        return ECG_TRIGGERS;
    } else if (_szGroup.compare("ECG_MALGLOBAL") == 0) {
        return ECG_MALGLOBAL;
    } else if (_szGroup.compare("ECG_COBERTURES") == 0) {
        return ECG_COBERTURES;
    } else if (_szGroup.compare("ECG_DYNAMIC_OBJECTS") == 0) {
        return ECG_DYNAMIC_OBJECTS;
    } else if (_szGroup.compare("ECG_EXPLOSIONS") == 0) {
        return ECG_EXPLOSIONS;
    } else if (_szGroup.compare("ECG_RAY_SHOOT") == 0) {
        return ECG_RAY_SHOOT;
    } else if (_szGroup.compare("ECG_RAY_SHOOT_PLAYER") == 0) {
        return ECG_RAY_SHOOT_PLAYER;
    } else if (_szGroup.compare("ECG_FORCE") == 0) {
        return ECG_FORCE;
    } else if (_szGroup.compare("ECG_CAMERA") == 0) {
        return ECG_CAMERA;
    } else if (_szGroup.compare("ECG_RAGDOLL") == 0) {
        return ECG_RAGDOLL;
    } else if (_szGroup.compare("ECG_RAGDOLL_PLAYER") == 0) {
        return ECG_RAGDOLL_PLAYER;
    } else if (_szGroup.compare("ECG_RAY_IA_GRAPH") == 0) {
        return ECG_RAY_IA_GRAPH;
    } else if (_szGroup.compare("ECG_VIGIA") == 0) {
        return ECG_VIGIA;
    } else if (_szGroup.compare("ECG_LIMITS") == 0) {
        return ECG_LIMITS;
    } else if (_szGroup.compare("ECG_STATIC_OBJECTS") == 0) {
        return ECG_STATIC_OBJECTS;
    } else {
        return 0;
    }
}

ECollisionGroup CPhysicsManager::GetCollisionGroup( const int _szGroup )
{
    if (_szGroup == 0) {
        return ECG_ESCENE;
    } else if (_szGroup == 1) {
        return ECG_PLAYER;
    } else if (_szGroup == 2) {
        return ECG_ENEMY;
    } else if (_szGroup == 3) {
        return ECG_TRIGGERS;
    } else if (_szGroup == 4) {
        return ECG_MALGLOBAL;
    } else if (_szGroup == 5) {
        return ECG_COBERTURES;
    } else if (_szGroup == 6) {
        return ECG_DYNAMIC_OBJECTS;
    } else if (_szGroup == 7) {
        return ECG_EXPLOSIONS;
    } else if (_szGroup == 8) {
        return ECG_RAY_SHOOT;
    } else if (_szGroup == 9) {
        return ECG_RAY_SHOOT_PLAYER;
    } else if (_szGroup == 10) {
        return ECG_FORCE;
    } else if (_szGroup == 11) {
        return ECG_CAMERA;
    } else if (_szGroup == 12) {
        return ECG_RAGDOLL;
    } else if (_szGroup == 13) {
        return ECG_RAGDOLL_PLAYER;
    } else if (_szGroup == 14) {
        return ECG_RAY_IA_GRAPH;
    } else if (_szGroup == 15) {
        return ECG_VIGIA;
    } else if (_szGroup == 16) {
        return ECG_LIMITS;
    } else if (_szGroup == 17) {
        return ECG_STATIC_OBJECTS;
    } else {
        return ECG_ESCENE;
    }
}

bool CPhysicsManager::CompareUserDatas(CPhysicUserData *_pUserData1, CPhysicUserData *_pUserData2)
{
    if ( _pUserData1 == _pUserData2 )
        return true;
    return false;
}

bool CPhysicsManager::CreateMeshFromXML(const std::string &FileName)
{
    m_FileName = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        LOGGER->AddNewLog(ELL_WARNING, "ERROR loading the file %s.\n", FileName);
        return false;
    }
    CXMLTreeNode  m = newFile["PhyX"];
    if (m.Exists()) {
        std::string folder =  m(0).GetPszProperty("folder");
        std::string path = "./assets/PhX/" + folder + "/";
        unsigned int count = (unsigned int)m.GetNumChildren();
        for (size_t i = 1; i < count; ++i) {
            //path viejo - "./assets/data/ases/barrioRico/"
            //path nuevo - "./assets/PhX/Fisicas_arturo/"
            //std::string path = "./assets/PhX/Fisicas_arturo/";
            std::string name = m(i).GetPszProperty("name");
            std::string file = path + name + ".ASE";
            // Guardar id elemento
            m_vIds.insert( std::pair<std::string, unsigned int>(name, i - 1) );
            /********************/
            CPhysicUserData * l_pUserData = new CPhysicUserData(name);
            l_pUserData->setName(name);
            l_pUserData->SetPaint(true);
            l_pUserData->SetColor(colGREEN);
            l_pUserData->SetGroup(ECG_ESCENE);
            m_vUsersData.push_back(l_pUserData);
            CPhysicActor * l_pSceneMesh = new CPhysicActor(l_pUserData);
            m_vActors.push_back(l_pSceneMesh);
            GetCookingMesh()->CreateMeshFromASE(file, name);
            l_pSceneMesh->AddMeshShape(GetCookingMesh()->GetPhysicMesh(name));
            AddPhysicActor(l_pSceneMesh);
        }
    }
    return true;
}