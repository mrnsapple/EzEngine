namespace ez::details
{
	template <typename F, typename ...Args>
	void ThreadPoolInternal::defer(F &&f, Args&& ...args)
	{
		this->_maintain_run.fetch_add(1, std::memory_order_relaxed);
		if (this->_post_workers.getSizeOfInactiveThread() > this->_post_workers.getSizeTasks())
			this->_post_workers.push([=]{
				f(args...);
			});
		else
			this->_dispatch_worker.push([=]{
				f(args...);
			});
		this->_maintain_run.fetch_sub(1, std::memory_order_relaxed);
	}

	template <typename F, typename ...Args>
	void ThreadPoolInternal::post(F &&f, Args&& ...args)
	{
		this->_post_workers.push([=]{
			f(args...);
		});
	}

	template <typename F, typename ...Args>
	void ThreadPoolInternal::dispatch(F &&f, Args&& ...args)
	{
		this->_dispatch_worker.push([=]{
			f(args...);
		});
	}
}