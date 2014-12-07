#include "StaticMesh.h"
#include "Texture/Texture.h"
#include "RenderableVertex/RenderableVertexs.h"
#include "GraphicsManager.h"
#include "RenderableVertex/VertexTypes.h"
#include "RenderableVertex/IndexedVertexs.h"
#include "Core/Core.h"
#include <cstdio>
#include "Utils/Logger.h"
#include "Texture\TextureManager.h"

#include "Core_Utils/MemLeaks.h"

CStaticMesh::CStaticMesh() : m_NumVertexs(0),
    m_NumFaces(0), m_deleted(false)
{
}

CStaticMesh::~CStaticMesh()
{
    if (!m_deleted) {
        Destroy();
        m_deleted = true;
    }
}

void CStaticMesh::Destroy()
{
    for (std::vector<CRenderableVertexs*>::iterator it = m_RVs.begin(); it != m_RVs.end(); ++it) {
        CHECKED_DELETE(*it);
    }
    m_RVs.clear();
    for (std::vector<std::vector<CTexture *>>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it) {
        for (std::vector<CTexture *>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {
            //CHECKED_DELETE(*it1);
        }
        it->clear();
    }
    m_Textures.clear();
}

bool CStaticMesh::Load (const std::string &FileName)
{
    m_RVs = std::vector<CRenderableVertexs*>();
    m_Textures = std::vector<std::vector<CTexture *>>();
    m_FileName = FileName;
    FILE *f;
    f = fopen(FileName.c_str(), "rb");
    if (f != NULL) {
        unsigned short header;
        fread(&header, sizeof header, 1, f);
        if (header == 0x55ff) {
            unsigned short n_mat;
            fread(&n_mat, sizeof n_mat, 1, f);
            std::vector<unsigned short> vertex_type;
            for (int i = 0; i < n_mat; ++i) {
                unsigned short aux;
                fread(&aux, sizeof aux, 1, f);
                vertex_type.push_back(aux);
                unsigned short n_text;
                fread(&n_text, sizeof n_text, 1, f);
                m_Textures.push_back(std::vector<CTexture*>());
                for (int j = 0; j < n_text; ++j) {
                    unsigned short aux_size;
                    fread(&aux_size, sizeof aux_size, 1, f);
                    char *buff = (char *)malloc(aux_size * sizeof(char) + 1);
                    size_t len = fread(buff, sizeof(char), aux_size + 1, f);
                    std::string aux_name(buff);
                    delete(buff);
                    CTexture *aux_text;
                    if (TEXTM->ExisteResource(aux_name)) {
                        aux_text = TEXTM->GetResource(aux_name);
                    } else {
                        aux_text = new CTexture();
                        aux_text->Load(aux_name);
                        TEXTM->AddResource(aux_name, aux_text);
                    }
                    m_Textures[i].push_back(aux_text);
                }
            }
            for (int i = 0; i < n_mat; ++i) {
                unsigned short n_vtxs;
                fread(&n_vtxs, sizeof n_vtxs, 1, f);
                int type_size;
                if (vertex_type[i] == TTEXTURE_NORMAL_VERTEX::GetVertexType())
                    type_size = sizeof(TTEXTURE_NORMAL_VERTEX);
                void* vertex_list = (void*)malloc(n_vtxs * type_size);
                fread(vertex_list, n_vtxs * type_size, 1, f);
                unsigned short n_idxs;
                fread(&n_idxs, sizeof n_idxs, 1, f);
                void* index_list = (void*)malloc(n_idxs * sizeof(unsigned short));
                fread(index_list , n_idxs * sizeof(unsigned short), 1, f);
                CRenderableVertexs *l_rv = NULL;
                if (vertex_type[i] == TTEXTURE_NORMAL_VERTEX::GetVertexType())
                    l_rv = new CIndexedVertexs<TTEXTURE_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
                m_RVs.push_back(l_rv);
                delete(vertex_list);
                delete(index_list);
            }
            unsigned short footer;
            fread(&footer, sizeof footer, 1, f);
            if (footer != 0xff55) {
                fclose(f);
                return false;
            } else
                fclose(f);
            return true;
        }
        fclose(f);
        return false;
    }
    return false;
}
bool CStaticMesh::ReLoad ()
{
    Destroy();
    m_deleted = false;
    return Load(m_FileName);
}
void CStaticMesh::Render (CGraphicsManager *RM)
{
    for (int i = 0; i < m_RVs.size(); ++i) {
        // TODO iterate m_textures
        m_Textures[i][0]->Activate(0);
        m_RVs[i]->Render(RM);
    }
}

