CXMLTreeNode NewXML;

if (NewXML.StartNewFile("test.xml"))
{
  // We fill the doc here
  NewXML.StartElement("Companies");
  NewXML.StartElement("Company");
  NewXML.WritePszProperty("name", "UAB");
  NewXML.WritePszProperty("location", "Marina 13");
  NewXML.StartElement("Workers");
  NewXML.WriteIntProperty("number", 2);
  NewXML.StartElement("worker");
  NewXML.WritePszProperty("name", "Gabriel");
  NewXML.WritePszProperty("surname", "Cirera");
  NewXML.WriteIntProperty("age", 27);
  NewXML.WriteFloatProperty("salary", 232.4f);
  NewXML.WriteBoolProperty("dead", true);
  NewXML.EndElement();
  NewXML.StartElement("worker");
  NewXML.WritePszProperty("name", "Toni");
  NewXML.WritePszProperty("surname", "Navarrete");
  NewXML.WriteIntProperty("age", 46);
  NewXML.WriteFloatProperty("salary", 122.3214f);
  NewXML.WriteBoolProperty("dead", false);
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndNewFile();

  CXMLTreeNode newFile;
  if (!newFile.LoadFile("test.xml"))
  {
    printf("ERROR loading the file.");
    return 0;
  }

  CXMLTreeNode  m = newFile["Workers"];
  if (m.Exists())
  {
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i)
    {
      const char* name = m(i).GetPszProperty("name");
      int iAge = m(i).GetIntProperty("age");
      float fSalary = m(i).GetFloatProperty("salary");
      bool bDead = m(i).GetBoolProperty("dead");

      printf("Name: %s, Age: %d, Salary: %f, Dead: %d\n", name, iAge, fSalary, bDead);
    }
  }
}