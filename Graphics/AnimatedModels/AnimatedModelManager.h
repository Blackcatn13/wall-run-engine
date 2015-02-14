#include "Utils\MapManager.h"
#include "AnimatedCoreModel.h"

#define MAXBONES 40

class CAnimatedInstanceModel;

class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
public:
    CAnimatedModelManager();
    ~CAnimatedModelManager();
    CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path);
    CAnimatedInstanceModel * GetInstance(const std::string &Name);
    void Load(const std::string &Filename);
};