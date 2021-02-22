/**
 * @file "Core/ThreadPool/Private/ThreadParallelMechanics.tpp"
 *
 * @author Alexandre Chetafi
**/

namespace ez::details
{
	template <typename F>
	void ThreadParallelMechanics::push(F &&f)
	{
		if (this->mustThreadStop())
			return;
		std::unique_lock<std::mutex>	_lock(this->_mtx_member_access);
		/* CRITICAL SECTION */
		this->_tasks.emplace_back(f);
	}
}