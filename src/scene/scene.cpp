#include "scene.h"

#include "material.h"
#include "window/window.h"

#include <string>


//void DeleteStaticRender(struct RenderContext *rc) {
//    free(rc->VAOs);
//    free(rc);
//};


namespace neith {

    //unsigned int Scene::getComponentTypeID() {
        //static unsigned int lastID = 0;
        //components.push_back(new Component());
        //return lastID++;
    //}
    //template<typename T>
    //unsigned int Scene::getComponentTypeID() {
        //static unsigned int componentID = getComponentTypeID();
        //return componentID;
    //}

    std::unordered_map<std::string, unsigned int> Scene::mEntityNames = {};
    std::vector<Entity*> Scene::mEntitys = {};

    Scene::Scene(int maxPrimitives) {
        //sp = new StaticPrimitives(maxPrimitives);
        mat = new Materials(maxPrimitives);
        cd = new Camera();
    }

    int Scene::AddEntity() {
        std::string entityName = "Entity." + std::to_string(mEntitys.size());
        return AddEntity(entityName);
    }

    int Scene::AddEntity(unsigned int parentID) {
        std::string entityName = "Entity." + std::to_string(mEntitys.size());
        return AddEntity(entityName, parentID);
    }

    int Scene::AddEntity(std::string &name) {
        int id = mEntitys.size();
        mEntitys.push_back(new Entity(id));
        mEntityNames.insert({name, id});
        return id;
    }

    int Scene::AddEntity(std::string &name, unsigned int parentID) {
        int id = mEntitys.size();
        mEntitys.push_back(new Entity(id, parentID));
        mEntityNames.insert({name, id});
        return id;
    }

    int Scene::AddEntity(std::string &name, std::string parentName) {
        return AddEntity(name, mEntityNames[parentName]);
    }

    Entity* Scene::GetEntity(unsigned int entityID) {
        return mEntitys.at(entityID);
    }

    Scene::~Scene() {
        delete mat;
        //delete sp;
        delete cd; 
    }
}
