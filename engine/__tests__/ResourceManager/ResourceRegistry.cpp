#include <gtest/gtest.h>
#include "ResourceManager.hpp"

#include "UniqueIDManager.hpp"
#include "UniqueID.hpp"
using namespace ez;

class Actor 
{
public:
    int i;
    std::string g;
};

TEST(RESOURCE_REGISTRY, Test_std_weak) {
    static auto x = UniqueIDManager::create();

    std::shared_ptr<UniqueID> id(new UniqueID);
    std::shared_ptr<UniqueID> idtwo;

    EXPECT_EQ(nullptr,idtwo);
    EXPECT_EQ(0,idtwo.use_count());

    EXPECT_NE(nullptr,id);
    //EXPECT_EQ(id,idfour);
	//EXPECT_NE(id, idfive);
}

TEST(RESOURCE_REGISTRY, Test_ADD) {
    static auto x = UniqueIDManager::create();

    ResourceRegistry *_registry =  new ResourceRegistry;

    std::shared_ptr<UniqueID>id =  _registry->Add("/filepath", "/binarypath/file",3,"obj");
    std::cout << "IDE:" << id.get() << std::endl;
    std::shared_ptr<UniqueID>idtwo =  _registry->Add("/filepath", "/binarypath/fileee",3,"obj");
    std::cout << "IDTWO:" << idtwo.get() << std::endl;
    std::shared_ptr<UniqueID>idthre =  _registry->Add("/filepatdh", "/binarypath/fileee",3,"obj");
    std::cout << "IDTRHE:" << idthre.get() << std::endl;
    std::shared_ptr<UniqueID>idfour =  _registry->Add("/filepatdh", "/binarypath/fileee",3,"obj");
    std::cout << "IDFOUR:" << idfour.get() << std::endl;

    EXPECT_EQ(1, _registry->GetNumResources());
    std::shared_ptr<UniqueID>idfive =  _registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj");
    EXPECT_EQ(2, _registry->GetNumResources()); 
    EXPECT_EQ(*id,*idtwo);
    EXPECT_EQ(*id,*idthre);
    EXPECT_EQ(*id,*idfour);
	EXPECT_NE(*id, *idfive);
}

TEST(RESOURCE_REGISTRY, Test_Remove) {
    static auto x = UniqueIDManager::create();

    ResourceRegistry *_registry =  new ResourceRegistry;

    std::shared_ptr<UniqueID>id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    std::shared_ptr<UniqueID>idtwo(_registry->Add("/filepath", "/binarypath/fileee",3,"obj"));
    EXPECT_EQ(1, _registry->GetNumResources());
    _registry->Remove(id);
    EXPECT_EQ(0, _registry->GetNumResources());
    std::shared_ptr<UniqueID>idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    EXPECT_EQ(1, _registry->GetNumResources());
}

TEST(RESOURCE_REGISTRY, Test_hasid) {
    static auto x = UniqueIDManager::create();

    ResourceRegistry *_registry =  new ResourceRegistry;
    std::shared_ptr<UniqueID> newid = std::shared_ptr<UniqueID>(new UniqueID);

    EXPECT_EQ(false, _registry->HasId(std::shared_ptr<UniqueID>()));
    EXPECT_EQ(false, _registry->HasId(newid));

    std::shared_ptr<UniqueID>id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    EXPECT_EQ(false, _registry->HasId(newid));
    EXPECT_EQ(true, _registry->HasId(id));

    std::shared_ptr<UniqueID>idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    EXPECT_EQ(true, _registry->HasId(idfive));

}

TEST(RESOURCE_REGISTRY, Test_getidfrompath) {
    static auto x = UniqueIDManager::create();

    ResourceRegistry *_registry =  new ResourceRegistry;
    std::shared_ptr<UniqueID>id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    std::shared_ptr<UniqueID>idtwo(_registry->Add("/filepatddh", "/binarypath/file",3,"obj"));

    std::shared_ptr<UniqueID>idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    

    EXPECT_EQ( std::shared_ptr<UniqueID>(), _registry->getIdfromPath("/filepa"));
    EXPECT_EQ( std::shared_ptr<UniqueID>(), _registry->getIdfromPath("/filepath"));
    EXPECT_EQ( idtwo, _registry->getIdfromPath("/filepatddh"));
    EXPECT_EQ( idfive, _registry->getIdfromPath("/filepadddtdh"));

}


TEST(RESOURCE_REGISTRY, Test_getidfrombinarypath) {
    static auto x = UniqueIDManager::create();

    ResourceRegistry *_registry =  new ResourceRegistry;
    std::shared_ptr<UniqueID>id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    std::shared_ptr<UniqueID>idtwo(_registry->Add("/filepath", "/binarypfath/file",3,"obj"));

    std::shared_ptr<UniqueID>idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    

    EXPECT_EQ( std::shared_ptr<UniqueID>(), _registry->getIdfromBinaryPath("/filepa"));
    EXPECT_EQ( std::shared_ptr<UniqueID>(), _registry->getIdfromBinaryPath("/binarypath/file"));
    EXPECT_EQ( idtwo, _registry->getIdfromBinaryPath("/binarypfath/file"));
    EXPECT_EQ( idfive, _registry->getIdfromBinaryPath("/binaryadewdepath/fileee"));
    EXPECT_NE( idfive, _registry->getIdfromBinaryPath("/binarypfath/file"));


}

TEST(RESOURCE_REGISTRY, Test_getpath) {
    static auto x = UniqueIDManager::create();
    std::shared_ptr<UniqueID> newid = std::shared_ptr<UniqueID>(new UniqueID);

    ResourceRegistry *_registry =  new ResourceRegistry;
    std::shared_ptr<UniqueID> id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    std::shared_ptr<UniqueID> idtwo(_registry->Add("/filedpath", "/binarypath/file",3,"obj"));


    std::shared_ptr<UniqueID> idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    

    EXPECT_EQ( "", _registry->GetPath(newid));
    EXPECT_EQ( "", _registry->GetPath(std::shared_ptr<UniqueID>()));
    EXPECT_NE( "/filepath", _registry->GetPath(id));
    EXPECT_EQ("/filedpath", _registry->GetPath(id));
    EXPECT_EQ("/filedpath", _registry->GetPath(idtwo));
    EXPECT_EQ("/filepadddtdh", _registry->GetPath(idfive));
}

TEST(RESOURCE_REGISTRY, Test_getbinarypath) {
    static auto x = UniqueIDManager::create();
    std::shared_ptr<UniqueID> newid = std::shared_ptr<UniqueID>(new UniqueID);

    ResourceRegistry *_registry =  new ResourceRegistry;
    std::shared_ptr<UniqueID> id(_registry->Add("/filepath", "/binarypath/file",3,"obj"));
    std::shared_ptr<UniqueID> idtwo(_registry->Add("/filepath", "/binarypfffath/file",3,"obj"));

    std::shared_ptr<UniqueID> idfive(_registry->Add("/filepadddtdh", "/binaryadewdepath/fileee",3,"obj"));
    

    EXPECT_EQ( "", _registry->GetBinaryDataPath(newid));
    EXPECT_EQ( "", _registry->GetBinaryDataPath(std::shared_ptr<UniqueID>()));
    EXPECT_NE("/binarypath/file", _registry->GetBinaryDataPath(id));
    EXPECT_EQ("/binarypfffath/file", _registry->GetBinaryDataPath(id));
    EXPECT_EQ("/binarypfffath/file", _registry->GetBinaryDataPath(idtwo));
    EXPECT_EQ( "/binaryadewdepath/fileee", _registry->GetBinaryDataPath(idfive));
}

