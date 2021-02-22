#include "../Public/PoolManager.hpp"
using namespace ez;


PoolManager *PoolManager::instance = nullptr;

PoolManager::PoolManager(std::size_t chunksPerBlock): _chunksPerBlock(chunksPerBlock)
{
}  


PoolManager *PoolManager::getNewInstance(std::size_t chunksPerBlock)
{
    instance = new PoolManager(chunksPerBlock);
    return instance;
} 


PoolManager *PoolManager::getInstance(std::size_t chunksPerBlock) 
{
    if (!instance)
        instance = getNewInstance(chunksPerBlock);
    return instance;
}

PoolManager::Node *PoolManager::GetPool(const std::size_t sz)
{
    _pools.read(_pools.head);

    for (PoolManager::Node *i = _pools.head; i != nullptr; i = i->next) {
        if (i->data->GetChunkSize() == sz && !i->data->isFull()) {
            return i;
        }
    }
    return CreatePool(sz);
}

PoolManager::Node * PoolManager::CreatePool(const std::size_t chunkSize)
{
    PoolAllocator *pool = new PoolAllocator(_chunksPerBlock, chunkSize);
    //Node node{pool, nullptr};
    Node *node = new Node;
    node->data = pool;
    node->next = nullptr;
    _pools.push(node);
    _amountPools +=1;
    return node;
}

void *PoolManager::Allocate(const std::size_t sz) noexcept
{
    void *memory;
    Node* node = GetPool(sz);    
    memory = (node->data)->Allocate(sz);
    return memory;
}

void PoolManager::Dellocate(void* data) noexcept
{

    for (PoolManager::Node *i = _pools.head; i != nullptr && !i->data->InPool(data); i = i->next);
}