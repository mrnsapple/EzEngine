/**
 * @file "Core/UniqueID/Public/UniqueIDManager.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_UNIQUEIDMANAGER_HPP
#define ENGINE_UNIQUEIDMANAGER_HPP

#include <map>
#include <unordered_set>
#include <memory>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace ez
{
	class UniqueID;
	class WeakNodeID;
	class WeakUniqueID;

	class UniqueIDManager : public std::enable_shared_from_this<UniqueIDManager>
	{
	public:
		friend class UniqueID;
		friend class WeakNodeID;
		friend class WeakUniqueID;
		using elem = unsigned long long;
		static std::shared_ptr<UniqueIDManager>	create();

		~UniqueIDManager() = default;
		UniqueIDManager(UniqueIDManager &) = delete;
		UniqueIDManager &operator=(UniqueIDManager &) = delete;

	private:
		UniqueIDManager() = default;
		elem	increaseCount();

		static std::shared_ptr<UniqueIDManager>	getPtr() noexcept;

		void	replace(UniqueID &dst, UniqueID &replace);
		void 	subscribe(UniqueID *);
		void 	unsubscribe(UniqueID *);

		/**
		 * Create or get the actual node of the unique id
		 * @return A node for the specific unique id, if the id exist
		 */

		WeakNodeID	*getNode(const UniqueID *);

		/**
		 * Delete the node
		 */

		void 	deleteNode(WeakNodeID *);

		/**
		 * Link the id to a node
		 */

		void	linkWeak(WeakNodeID *, const UniqueID *);

		/**
		 * Unlink the node and id
		 */

		void 	unlinkWeak(WeakNodeID *);
		void	unlinkWeak(const UniqueID *);

		/**
		 * Compare that the linked id of the node, is the same as the second paramter
		 * @return
		 */

		bool	compare(WeakNodeID *, const UniqueID *) const noexcept;

		/**
		 * The validity of a node is dependant of the existence of the id
		 * @return
		 */

		bool	isWeakValid(WeakNodeID *) const noexcept;

	private:
		std::atomic<elem>					_count {1};
		std::map<elem, const UniqueID *>	_uniqueIdList;
		std::mutex							_mutex;
		mutable std::shared_mutex			_weakRefMutex;
		std::mutex							_weakMoveMutex;

		std::map<const UniqueID *, WeakNodeID *>	_weak;
		std::map<WeakNodeID *, const UniqueID *>	_weakRef;
		std::unordered_set<WeakNodeID *>			_refNode;
		std::unordered_set<WeakNodeID *>			_refToBeDeleted;
	};
}

#endif //ENGINE_UNIQUEIDMANAGER_HPP
