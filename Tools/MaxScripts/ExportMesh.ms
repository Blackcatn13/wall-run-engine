function ExportMesh Obj filename =
(
	local l_NumMaterials 
	local l_Vtxs = #()
	local l_Idxs = #()
	local file=fopen filename "wb"

	WriteShort file 0x55ff #unsigned

	
	-- Obtener numero de materiales del objeto
	
	if classof(Obj.material) == Multimaterial then 
	(  
		format "el objeto tiene aplicado un multimaterial\n"
		l_NumMaterials = Obj.material.numsubs
	) else 
	(  
		format "el objeto tiene aplicado un material standard\n"
		l_NumMaterials = 1 --Obj.material -->Si no es multimaterial solo tiene 1?
	)
	
	
	WriteShort file l_NumMaterials #unsigned
	for i = 1 to l_NumMaterials do
	(
		WriteShort file 0x13 #unsigned --poner mascara
		WriteShort file 1 #unsigned --numero texturas
		ExportMaterial Obj.material[i] file
	)	
	
	GenerateVertexs Obj l_Vtxs l_Idxs
	for idMaterial = 1 to l_NumMaterials do
	(	
		--format "l_NumMaterials %\n" l_NumMaterials
		--format "%\n" l_Vtxs
		local numVertex = l_Vtxs[idMaterial].count
		local numIndexes = l_Idxs[idMaterial].count
		--format "num vertex = %\n" numVertex
		--format "num index = %\n" numIndexes
		WriteShort file numVertex #unsigned
		ExportVertexs l_Vtxs file idMaterial     --Estos arrays en realiad son tridimensionales [][][]
		WriteShort file numIndexes #unsigned
		ExportIndexs l_Idxs file idMaterial
	)
	
	WriteShort file 0xff55 #unsigned
	
	fflush file
	fclose file
)

function ExportMaterial _Material File =
(
	format "[ExportMaterial] mat name %\n" _Material
	for  i=1 to 1 do --numero de texturas
	(
		local fileName = (openBitMap _Material.diffuseMap.bitmap.filename).fileName 
		format "path %\n" filename
		local relPath = pathConfig.convertPathToRelativeTo fileName "C:\\users\\marc\desktop\UABEngine\UABEngine\bin\Videogame\\"
		format "rel path %\n" relPath
		local numChars = relPath.count
		WriteShort File numChars #unsigned
		WriteString File relPath 
		
	)
)

function ExportVertexs arrayVertex File idMaterial =
(
	--local numVertexs = arrayVertex.count
	--WriteString File "<vertexs numVertexs = \" % \" >\n" numVertexs
	
	for  j=1 to arrayVertex[idMaterial].count do
	(                 
		for  k=1 to arrayVertex[idMaterial][j].count do
		(
			WriteFloat File arrayVertex[idMaterial][j][k]
		)
	)
)

function ExportIndexs arrayIndex File idMaterial =
(
	for  j=1 to arrayIndex[idMaterial].count do
	(   
		WriteShort File arrayIndex[idMaterial][j] #unsigned
	)
)

/*----------------------------------------------*/

function AreVertexsEqual Vtx1 Vtx2 =
(
	if Vtx1.count!=Vtx2.count then
	(
		--format "[AreVertexEquals] false\n"
		return false
	)
	else
	(
		for i=1 to Vtx1.count do
		(
			if Vtx1[i]!=Vtx2[i] then
			(
				--format "[AreVertexEquals] false\n"
				return false
			)
		)
		--format "[AreVertexEquals] true\n"
		return true
	)
)

function FindVertex Vtxs Vtx =
(
	--format "[FindVertex] looking for %\n" Vtx
	local pos=-1
	for i=1 to Vtxs.count do
	(
		--format "[FindVertex] Compare with %\n" Vtxs[i]
		if AreVertexsEqual Vtxs[i] Vtx then
		(
			--format "[FindVertex] returning %\n" i
			pos = i
			return pos
		)
	)
	return pos
)

/*---------------------------------------*/
function AddVertex vertex vrtxs idMaterial =
(
	--format "looking for equality of %\n" vertex
	
	while vrtxs.count<idMaterial do
	(
		append vrtxs (#())
	)
	local pos=FindVertex vrtxs[idMaterial] vertex
	--format "[AddVertex] to if\n"
	if pos==-1 then
	(
		append vrtxs[idMaterial] vertex
		return vrtxs[idMaterial].count
	)
	else
	(
		--format "[AddVertex] returning %\n" pos
		return pos
	)
)

function AddIndex index idxs idMaterial =
(
	while idxs.count<idMaterial do
	(
		append idxs (#())
	)
	
	append idxs[idMaterial] (index-1)
)

function CreateFullVertex vtx n tu1 tu2 col tn bn mater=
(
	--TODO comprobar tipo de material
	local vertex=#()
	append vertex vtx[1]
	append vertex vtx[3]
	append vertex vtx[2]
	append vertex n[1]
	append vertex n[3]
	append vertex n[2]
	append vertex tu1[1]
	append vertex tu1[2]
	return vertex
)

function GenerateVertexs Obj vtxs idxs=
(
	local numFaces = getNumFaces Obj
	--format "num faces %\n" numFaces
	for face=1 to numFaces do
	(
		local aFace=getFace Obj face
		local vtx1=getVert Obj aFace.z
		local vtx2=getVert Obj aFace.y
		local vtx3=getVert Obj aFace.x
		
		local Normal1 = getNormal Obj aFace.z
		local Normal2 = getNormal Obj aFace.y
		local Normal3 = getNormal Obj aFace.x
		
		local IdxsMap= meshop.getMapFace Obj 1 face
		local TUVMap1 =  meshop.getMapVert Obj 1 IdxsMap.z
		local TUVMap2 =  meshop.getMapVert Obj 1 IdxsMap.y
		local TUVMap3 =  meshop.getMapVert Obj 1 IdxsMap.x
		
		local totalVertex1 = CreateFullVertex vtx1 Normal1 TUVMap1 0 0 0 0 0
		local totalVertex2 = CreateFullVertex vtx2 Normal2 TUVMap2 0 0 0 0 0
		local totalVertex3 = CreateFullVertex vtx3 Normal3 TUVMap3 0 0 0 0 0
		
		local idMaterial = getFaceMatID Obj face
		
		if classof(Obj.material)==Standardmaterial then
		(
			idMaterial=1
		)
		
		local index1=AddVertex totalVertex1 vtxs idMaterial
		local index2=AddVertex totalVertex2 vtxs idMaterial
		local index3=AddVertex totalVertex3 vtxs idMaterial
	
		AddIndex index1 idxs idMaterial
		AddIndex index2 idxs idMaterial
		AddIndex index3 idxs idMaterial
	
	)
)

ExportMesh $ "C:\\exported.mesh"