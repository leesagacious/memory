
/*
   内核线程kswapd, 内存不足的时候 用来回收页面
   
   好，下面看看它的生命周期
*/
int kswapd_run(int nid)
{
	pg_data_t *pgdat = NODE_DATA(nid);
	int ret = 0;

	if (pgdat->kswapd)
		return 0;

	pgdat->kswapd = kthread_run(kswapd, pgdat, "kswapd%d", nid);
	if (IS_ERR(pgdat->kswapd)) {
		/* failure at boot is fatal */
		BUG_ON(system_state == SYSTEM_BOOTING);
		pr_err("Failed to start kswapd on node %d\n", nid);
		ret = PTR_ERR(pgdat->kswapd);
		pgdat->kswapd = NULL;
	}
	return ret;
}
/*
USER     GROUP    COMMAND          PID   PPID
0        0        init                 1     0
0        0        kthreadd             2     0
0        0        ksoftirqd/0          3     2
0        0        kworker/0:0          4     2
0        0        kworker/u:0          5     2
0        0        khelper              6     2
0        0        kdevtmpfs            7     2
0        0        netns                8     2
0        0        kworker/u:1          9     2
0        0        sync_supers        285     2
0        0        kblockd            289     2
0        0        khubd              313     2
0        0        rpciod             438     2
0        0        kswapd0            448     2    用于回收页面的内核线程
*/
