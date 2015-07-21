#include "AI\WPManager.h"
#include "XML\XMLTreeNode.h"

CWPManager::CWPManager()
  : m_fileName ("") {
}

CWPManager::~CWPManager() {
  for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
    for (std::vector<WP *>::iterator it2 = it->second->m_Waypoints.begin(); it2 != it->second->m_Waypoints.end(); ++it2) {
      for (std::vector<Link *>::iterator it3 = (*it2)->m_LinkList.begin(); it3 != (*it2)->m_LinkList.end(); ++it3) {
        CHECKED_DELETE((*it3));
      }
      (*it2)->m_LinkList.clear();
      CHECKED_DELETE((*it2)->m_Padre);
    }
    it->second->m_Waypoints.clear();
    CHECKED_DELETE(it->second);
  }
  m_Resources.clear();
  // Destroy();
}

void CWPManager::Load(std::string file) {
  m_fileName = file;
  Load();
}

void CWPManager::Reload() {
  Destroy();
  Load();
}


int CWPManager::FindClosestWaypoint(std::string ZONEName, Vect3f Position) {
  int l_closestId = 10;
  float l_closestDistance = 9999.0;
  ZONE *l_currentZone = GetResource(ZONEName);
  for (std::vector<WP *>::iterator it = l_currentZone->m_Waypoints.begin(); it != l_currentZone->m_Waypoints.end(); ++it) {
    //(*it)->m_id
    float l_WaypointDistance = GetDistance((*it)->m_pos, Position);
    if (l_WaypointDistance < l_closestDistance) {
      l_closestId = (*it)->m_id;
      l_closestDistance = l_WaypointDistance;
    }
  }
  return l_closestId;
}

int CWPManager::CalcularSiguienteWaypoint(int wp_actual, int wp_destino, std::string ZONEName) {
  //reseteamos WP Struct
  ResetWPStruct(ZONEName);
  //inputs de Test
  ZONE *l_currentZone = GetResource(ZONEName);
  int id_origen = wp_actual;
  int id_destino = wp_destino;
  bool l_Busquedafinalizada = false;
  //caso ya en el destino
  if (id_origen == id_destino) {
    return id_origen;
  }
  //Vect3f posicionDestino = Vect3f(7.000, 0.000, 7.000);
  //creamos vector de nodos abiertos
  std::vector<WP *> l_openVector;
  //añadimos a vectores abiertos el nodo por defecto
  l_openVector.push_back(l_currentZone->m_Waypoints[id_origen]);
  //calculamos G H y F para nodo abierto inicial
  float temp_H = CalcularH(id_origen, id_destino, l_currentZone);
  float temp_G = 0.0;
  float temp_F = temp_H + temp_G;
  float l_pesoRecorrido = 0.0;
  //modificamos los datos del nodo origen 0
  l_currentZone->m_Waypoints[id_origen]->m_H = temp_H;
  l_currentZone->m_Waypoints[id_origen]->m_G = temp_G;
  l_currentZone->m_Waypoints[id_origen]->m_F = temp_F;
  //calculamos todos los vecinos de nodo inicial
  for (std::vector<Link *>::iterator it = l_currentZone->m_Waypoints[id_origen]->m_LinkList.begin(); it != l_currentZone->m_Waypoints[id_origen]->m_LinkList.end(); ++it) {
    if (!l_currentZone->m_Waypoints[(*it)->id]->m_cerrado) {
      //9999 quiere decir que no se ha calculado H aun para este nodo
      if (l_currentZone->m_Waypoints[(*it)->id]->m_H == 9999.0) {
        temp_H = CalcularH((*it)->id, id_destino, l_currentZone);
        l_currentZone->m_Waypoints[(*it)->id]->m_H = temp_H;
      } else {
        temp_H = l_currentZone->m_Waypoints[(*it)->id]->m_H;
      }
      l_pesoRecorrido = (*it)->weight + temp_G;
      temp_F = temp_H + l_pesoRecorrido;
      if (temp_F < l_currentZone->m_Waypoints[(*it)->id]->m_F) {
        l_currentZone->m_Waypoints[(*it)->id]->m_G = l_pesoRecorrido;
        l_currentZone->m_Waypoints[(*it)->id]->m_F = temp_F;
        l_currentZone->m_Waypoints[(*it)->id]->m_Padre = l_currentZone->m_Waypoints[id_origen];
      }
      if ((*it)->id == id_destino) {
        l_Busquedafinalizada = true;
        return CalcularPrimerWaypoint((*it)->id, l_currentZone);
      }
      l_openVector.push_back(l_currentZone->m_Waypoints[(*it)->id]);
    }
  }
  //cerramos el vector actual
  l_currentZone->m_Waypoints[id_origen]->m_cerrado = true;
  //sacamos el nodo origen cerrado del vector de abiertos
  for (std::vector<WP *>::iterator it = l_openVector.begin(); it != l_openVector.end(); ++it) {
    if ( (*it)->m_id == id_origen) {
      l_openVector.erase(it);
      break;
    }
  }
  //hacemos el bucle para el resto de nodos, siempre que hayan nodos abiertos y no hayamos encontremos la salida
  while ((l_openVector.size() > 0) && (!l_Busquedafinalizada)) {
    int temp_F_Busqueda = 1000.0;
    int temp_id = 99;
    //cogemos el nodo abierto con una F menor
    for (std::vector<WP *>::iterator it = l_openVector.begin(); it != l_openVector.end(); ++it) {
      if ( (*it)->m_F < temp_F_Busqueda) {
        temp_id = (*it)->m_id;
        temp_F_Busqueda = (*it)->m_F;
      }
    }
    id_origen = temp_id;
    temp_G = l_currentZone->m_Waypoints[id_origen]->m_G;
    //repetimos los pasos que con el nodo original
    for (std::vector<Link *>::iterator it = l_currentZone->m_Waypoints[id_origen]->m_LinkList.begin(); it != l_currentZone->m_Waypoints[id_origen]->m_LinkList.end(); ++it) {
      if (!l_currentZone->m_Waypoints[(*it)->id]->m_cerrado) {
        l_pesoRecorrido = (*it)->weight + temp_G;
        if (l_currentZone->m_Waypoints[(*it)->id]->m_H == 9999.0) {
          temp_H = CalcularH((*it)->id, id_destino, l_currentZone);
          l_currentZone->m_Waypoints[(*it)->id]->m_H = temp_H;
        } else {
          temp_H = l_currentZone->m_Waypoints[(*it)->id]->m_H;
        }
        temp_F = temp_H + l_pesoRecorrido;
        if (temp_F < l_currentZone->m_Waypoints[(*it)->id]->m_F) {
          l_currentZone->m_Waypoints[(*it)->id]->m_G = l_pesoRecorrido;
          l_currentZone->m_Waypoints[(*it)->id]->m_F = temp_F;
          l_currentZone->m_Waypoints[(*it)->id]->m_Padre = l_currentZone->m_Waypoints[id_origen];
        }
        if ((*it)->id == id_destino) {
          l_Busquedafinalizada = true;
          return CalcularPrimerWaypoint((*it)->id, l_currentZone);
        }
        //miramos que l_currentZone->m_Waypoints[(*it)->id] no esté ya en openVectorf
        bool idExist = false;
        for (std::vector<WP *>::iterator it2 = l_openVector.begin(); it2 != l_openVector.end(); ++it2) {
          if ( (*it2)->m_id == l_currentZone->m_Waypoints[(*it)->id]->m_id) {
            idExist = true;
            break;
          }
        }
        if (!idExist) l_openVector.push_back(l_currentZone->m_Waypoints[(*it)->id]);
      }
    }
    //cerramos el vector actual
    l_currentZone->m_Waypoints[id_origen]->m_cerrado = true;
    //sacamos el nodo origen cerrado del vector de abiertos
    for (std::vector<WP *>::iterator it = l_openVector.begin(); it != l_openVector.end(); ++it) {
      if ( (*it)->m_id == id_origen) {
        l_openVector.erase(it);
        break;
      }
    }
  }
  return 9999;
}

int CWPManager::CalcularPrimerWaypoint(int id_entrada, ZONE *currentZone) {
  ZONE *l_currentZone = currentZone;
  int l_tempid = id_entrada;
  int LoopProtector = 50;
  while ((l_currentZone->m_Waypoints[l_tempid]->m_Padre != NULL) && (l_currentZone->m_Waypoints[l_tempid]->m_Padre->m_Padre != NULL) && (LoopProtector > 0)) {
    l_tempid = l_currentZone->m_Waypoints[l_tempid]->m_Padre->m_id;
    --LoopProtector;
  }
  return l_tempid;
}

float CWPManager::CalcularH(int id_origen, int id_destino, ZONE *currentZone) {
  float l_H = GetDistance(currentZone->m_Waypoints[id_origen]->m_pos, currentZone->m_Waypoints[id_destino]->m_pos);
  return l_H;
}

Vect3f CWPManager::GetWaypointPosition(int id, std::string ZONEName) {
  ZONE *l_currentZone = GetResource(ZONEName);
  return l_currentZone->m_Waypoints[id]->m_pos;
}

float CWPManager::GetDistance(Vect3f pos1, Vect3f pos2) {
  float l_distance = pow((pos1.x - pos2.x), 2) + pow((pos1.y - pos2.y), 2) + pow((pos1.z - pos2.z), 2);
  float l_result_distance = sqrt(l_distance);
  return l_result_distance;
}

void CWPManager::Load() {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(m_fileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["wp"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m.getNextChild();
        ZONE *newZONE = new ZONE();
        int count2 = nodeChild.GetNumChildren();
        std::string ZONEName = nodeChild.GetPszISOProperty("name", "");
        for (int j = 0; j < count2; ++j) {
          CXMLTreeNode nodeChild1 = nodeChild.getNextChild();
          std::string name = nodeChild.GetName();
          if (name == "waypoint") {
            WP *newWP = new WP();
            newWP->m_id = nodeChild.GetIntProperty("id", 0);
            newWP->m_pos = nodeChild.GetVect3fProperty("pos", Vect3f(0, 0, 0));
            newWP->m_cerrado = false;
            newWP->m_F = 9999.0;
            newWP->m_G = 9999.0;
            newWP->m_H = 9999.0;
            newWP->m_Padre = NULL;
            int count3 = nodeChild.GetNumChildren();
            for (int k = 0; k < count3; ++k) {
              CXMLTreeNode nodeChild2 = nodeChild1.getNextChild();
              Link *newLink = new Link();
              newLink->id = nodeChild2.GetIntProperty("id", 0);
              newLink->weight = nodeChild2.GetIntProperty("weight", 0);
              newWP->m_LinkList.push_back(newLink);
            }
            newZONE->m_Waypoints.push_back(newWP);
          }
        }
        AddResource(ZONEName, newZONE);
      }
    }
  }
}

void CWPManager::ResetWPStruct(std::string ZONEName) {
  ZONE *l_currentZone = GetResource(ZONEName);
  for (std::vector<WP *>::iterator it = l_currentZone->m_Waypoints.begin(); it != l_currentZone->m_Waypoints.end(); ++it) {
    (*it)->m_cerrado = false;
    (*it)->m_F = 9999.0;
    (*it)->m_G = 9999.0;
    (*it)->m_H = 9999.0;
    (*it)->m_Padre = NULL;
  }
}




