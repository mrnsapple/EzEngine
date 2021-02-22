/**
 * @file "Core/ThreadPool/Private/ThreadSequentialMechanics.tpp"
 *
 * @author Alexandre Chetafi
**/

namespace ez::details
{
	template <typename F>
	void ThreadSequentialMechanics::push(F &&f)
	{
		if (this->mustThreadStop())
			return;
		std::unique_lock<std::mutex>	_lock(this->_mutex);
		this->_tasks.emplace_back(f);
	}
}