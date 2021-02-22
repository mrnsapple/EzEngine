/**
 * @file "Core/UniqueID/Private/UniqueIDManager.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../Public/UniqueIDManager.hpp"
#include "../Public/UniqueID.hpp"
#include "WeakNodeID.hpp"

namespace ez
{
	namespace {
		std::shared_ptr<UniqueIDManager>	_manager {nullptr}; //TODO remove when initialisator available
	}

	std::shared_ptr<UniqueIDManager> UniqueIDManager::create()
	{
		if (!_manager)
			_manager = std::shared_ptr<UniqueIDManager>(new UniqueIDManager());
		return _manager;
	}

	void UniqueIDManager::replace(UniqueID &dst, UniqueID &sender)
	{
		std::unique_lock<std::mutex>		_lock(this->_mutex);
		/* CRITICAL SECTION */
		auto _it = this->_uniqueIdList.find(sender._id);
		if (_it != this->_uniqueIdList.end()) {
			this->_uniqueIdList.insert_or_assign(_it->first, dst.getThis());
			dst.setID(sender._id);
		}
		_lock.unlock();
		sender._id = 0;
	}

	UniqueIDManager::elem UniqueIDManager::increaseCount()
	{
		return this->_count.fetch_add(1, std::memory_order_relaxed);
	}

	std::shared_ptr<UniqueIDManager> UniqueIDManager::getPtr() noexcept
	{
		return _manager;
	}

	void UniqueIDManager::subscribe(UniqueID *ptr)
	{
		auto _id = this->increaseCount();
		std::unique_lock<std::mutex>	_lock(this->_mutex);
		/* CRITICAL SECTION */
		this->_uniqueIdList.insert({_id, ptr});
		_lock.unlock();
		ptr->setID(_id);
	}

	void UniqueIDManager::unsubscribe(UniqueID *ptr)
	{
		std::unique_lock<std::mutex>	_lock(this->_mutex);
		/* CRITICAL SECTION */
		this->_uniqueIdList.erase(ptr->_id);
		_lock.unlock();
		this->unlinkWeak(ptr);
	}

	WeakNodeID *UniqueIDManager::getNode(const UniqueID *id)
	{
		std::shared_lock	_lock(this->_weakRefMutex);
		auto it = _weak.find(id);
		if (it != _weak.end())
			return *(_refNode.find(it->second));
		else {
			_lock.unlock();
			auto _ptr = new WeakNodeID;
			this->linkWeak(_ptr, id);
			std::unique_lock	_lockWrite(this->_weakMoveMutex);
			_refNode.insert(_ptr);
			return _ptr;
		}
		return nullptr;
	}

	void UniqueIDManager::deleteNode(WeakNodeID *node)
	{
		this->unlinkWeak(node);
		if (node->use_count() > 0)
			return;
		std::unique_lock	_lockWrite(this->_weakMoveMutex);
		auto n = _refNode.extract(node);
		_refToBeDeleted.insert(std::move(n));
	}

	void UniqueIDManager::linkWeak(WeakNodeID *node, const UniqueID *id)
	{
		std::unique_lock	_lockWrite(this->_weakRefMutex);
		_weak.insert({id, node});
		_weakRef.insert({node, id});
	}

	void UniqueIDManager::unlinkWeak(WeakNodeID *node)
	{
		node->setInvalid();
		std::unique_lock	_lockWrite(this->_weakRefMutex);
		auto it = _weakRef.find(node);
		const UniqueID *_ptr = nullptr;

		if (it != _weakRef.end()) {
			_ptr = it->second;
			_weakRef.erase(it);
		}
		if (_ptr) {
			auto itt = _weak.find(_ptr);
			if (itt == _weak.end())
				_weak.erase(itt);
		}
	}

	void UniqueIDManager::unlinkWeak(const UniqueID *id)
	{
		std::unique_lock	_lockWrite(this->_weakRefMutex);
		auto it = _weak.find(id);
		WeakNodeID	*node = nullptr;
		if (it != _weak.end()) {
			node = it->second;
			_weak.erase(it);
		}
		if (node) {
			node->setInvalid();
			auto itt = _weakRef.find(node);
			if (itt != _weakRef.end())
				_weakRef.erase(itt);
		}
	}

	bool UniqueIDManager::compare(WeakNodeID *node, const UniqueID *id) const noexcept
	{
		std::shared_lock	_lock(this->_weakRefMutex);
		auto it = _weakRef.find(node);
		return it != _weakRef.cend() && *id != *(it->second);
	}

	bool UniqueIDManager::isWeakValid(WeakNodeID *node) const noexcept
	{
		std::shared_lock	_lock(this->_weakRefMutex);
		auto it = _weakRef.find(node);
		return it != _weakRef.cend();
	}
}