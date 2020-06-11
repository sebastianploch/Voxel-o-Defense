#pragma once
#include "Model.h"
#include "Mesh.h"

class Enemy : public Model
{
public:
    virtual std::shared_ptr<Enemy> Clone() = 0; //Enemy pure virtual function for creating clones of 
private:

};

class eZombie : public Enemy
{
public:
    
    eZombie()
    {
        this->SetMeshReference(zombieMeshes); 
    }

    std::shared_ptr <Enemy> Clone() override
    {
        return std::make_shared<eZombie>(*this);
    }
    static std::vector<Mesh> zombieMeshes;
};

class eSpider : public Enemy
{
public:
    eSpider() 
    { 
        this->SetMeshReference(spiderMeshes);
    }

    std::shared_ptr <Enemy> Clone() override
    {
        return std::make_shared<eSpider>(*this);
    }
    static std::vector<Mesh> spiderMeshes;
};

class eSkeleton : public Enemy
{
public:
    eSkeleton() 
    {
        this->SetMeshReference(skeletonMeshes); 
    }

    std::shared_ptr <Enemy> Clone() override
    {
        return std::make_shared<eSkeleton>(*this);
    }
    static std::vector<Mesh> skeletonMeshes;
};

class EnemyFactory //Factory
{
private:
    std::unordered_map<TypeOfMonster, std::vector<std::shared_ptr<Enemy>>, std::hash<int> > m_enemies;
public:
    EnemyFactory() = default;
    EnemyFactory(const EnemyFactory&) = default;
    EnemyFactory(Microsoft::WRL::ComPtr <ID3D11Device1> device)
    {
        std::shared_ptr<eZombie> zombie1 = std::make_shared<eZombie>();
        zombie1->Initialise("Resources/Models/Mesh/zombie1/zombie.obj", device.Get());
        m_enemies[ZOMBIE].push_back(zombie1);

        std::shared_ptr<eZombie> zombie2 = std::make_shared<eZombie>();
        zombie2->Initialise("Resources/Models/Mesh/zombie2/zombie2.obj", device.Get());
        m_enemies[ZOMBIE].push_back(zombie2);

        std::shared_ptr<eZombie> zombie3 = std::make_shared<eZombie>();
        zombie3->Initialise("Resources/Models/Mesh/zombie3/zombie3.obj", device.Get());
        m_enemies[ZOMBIE].push_back(zombie3);

        std::shared_ptr<eZombie> zombie4 = std::make_shared<eZombie>();
        zombie4->Initialise("Resources/Models/Mesh/zombie4/zombie4.obj", device.Get());
        m_enemies[ZOMBIE].push_back(zombie4);

        std::shared_ptr<eZombie> zombie5 = std::make_shared<eZombie>();
        zombie5->Initialise("Resources/Models/Mesh/zombie5/zombie5.obj", device.Get());
        m_enemies[ZOMBIE].push_back(zombie5);

        std::shared_ptr<eZombie> bigZombie = std::make_shared<eZombie>();
        bigZombie->Initialise("Resources/Models/Mesh/bigzombie1/bigzombie.obj", device.Get());
        m_enemies[ZOMBIE].push_back(bigZombie);

        std::shared_ptr<eSkeleton> skeleton1 = std::make_shared<eSkeleton>();
        skeleton1->Initialise("Resources/Models/Mesh/skeleton1/skeleton.obj", device.Get());
        m_enemies[SKELETON].push_back(skeleton1);

        std::shared_ptr<eSkeleton> skeleton2 = std::make_shared<eSkeleton>();
        skeleton2->Initialise("Resources/Models/Mesh/skeleton2/skeleton2.obj", device.Get());
        m_enemies[SKELETON].push_back(skeleton2);

        std::shared_ptr<eSkeleton> skeleton3 = std::make_shared<eSkeleton>();
        skeleton3->Initialise("Resources/Models/Mesh/skeleton3/skeleton3.obj", device.Get());
        m_enemies[SKELETON].push_back(skeleton3);

        std::shared_ptr<eSpider> spider1 = std::make_shared<eSpider>();
        spider1->Initialise("Resources/Models/Mesh/spider1/spider.obj", device.Get());
        m_enemies[SPIDER].push_back(spider1);

        std::shared_ptr<eSpider> spider2 = std::make_shared<eSpider>();
        spider2->Initialise("Resources/Models/Mesh/spider2/spider2.obj", device.Get());
        m_enemies[SPIDER].push_back(spider2);

        std::shared_ptr<eSpider> spider3 = std::make_shared<eSpider>();
        spider3->Initialise("Resources/Models/Mesh/spider3/spider3.obj", device.Get());
        m_enemies[SPIDER].push_back(spider3);
    }
    std::shared_ptr<Enemy> CreateEnemy(TypeOfMonster enemyType)
    {
        int v1 = 0;
        if (enemyType == ZOMBIE)
        {
            v1 = rand() % 5;
        }
        if (enemyType == SKELETON)
        {
            v1 = rand() % 2;
        }
        if (enemyType == SPIDER)
        {
        v1 = rand() % 2;
        }
        return m_enemies[enemyType][v1]->Clone();
    }
};