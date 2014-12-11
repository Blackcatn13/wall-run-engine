class CAnimatedInstanceModel;

class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
public:
	virtual CAnimatedModelManager();
	virtual ~CAnimatedModelManager();
	CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path);
	CAnimatedInstanceModel * GetInstance(const std::string &Name);
	void Load(const std::string &Filename);
};