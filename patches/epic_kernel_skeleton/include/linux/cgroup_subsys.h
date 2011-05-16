/* Add subsystem definitions of the form SUBSYS(<name>) in this
 * file. Surround each one by a line of comment markers so that
 * patches don't collide
 */

/* */

/* */

#ifdef CONFIG_CPUSETS
SUBSYS(cpuset)
#endif

/* */

SUBSYS(debug)

/* */

#ifdef CONFIG_CGROUP_NS
SUBSYS(ns)
#endif

/* */

SUBSYS(cpu_cgroup)

/* */

SUBSYS(cpuacct)

/* */

#ifdef CONFIG_CGROUP_MEM_RES_CTLR
SUBSYS(mem_cgroup)
#endif

/* */

#ifdef CONFIG_CGROUP_DEVICE
SUBSYS(devices)
#endif

/* */

SUBSYS(freezer)

/* */

#ifdef CONFIG_NET_CLS_CGROUP
SUBSYS(net_cls)
#endif

/* */
