#ifndef __SYSMGR_JSON_DEF_H_
#define __SYSMGR_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#include "ADCmnDevTypes.h"
#define SYSMGR_JSON_PORT_NUMBER    ADCMN_PORT_SYSMGR
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SYSMGR_RPC_TYPES_T
{
	EJSON_SYSMGR_RPC_GET_MAC_ADDR=0,
	EJSON_SYSMGR_RPC_SET_MAC_ADDR,
	EJSON_SYSMGR_RPC_GET_ETH_COUNT, //returns number of eth interfaces available on the device
	EJSON_SYSMGR_RPC_GET_ETH_NAME,

	EJSON_SYSMGR_RPC_GET_LOADINFO,
	EJSON_SYSMGR_RPC_GET_MEMINFO,
	EJSON_SYSMGR_RPC_GET_CPUINFO,
	EJSON_SYSMGR_RPC_GET_DEV_OP,
	EJSON_SYSMGR_RPC_SET_DEV_OP,
	EJSON_SYSMGR_RPC_GET_FMWVER,
	EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM, //returns current booted partition brbox1 or brbox2
	EJSON_SYSMGR_RPC_SET_FMWUPDATE,   //trigger fmw update
	EJSON_SYSMGR_RPC_SET_DOWNLOADFTP,
	EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP,
	EJSON_SYSMGR_RPC_GET_ASYNCTASK,
	EJSON_SYSMGR_RPC_GET_DEVTYPE,    //read the board or device type
	EJSON_SYSMGR_RPC_GET_HOSTNAME,
	EJSON_SYSMGR_RPC_SET_HOSTNAME,
	EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP, //internet ip
	EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME, //set default hostname
	EJSON_SYSMGR_RPC_SET_UPDATE_LOG, //reads BRBOX specific log lines from /tmp/messages and fills in vectorList
	EJSON_SYSMGR_RPC_GET_LOG_COUNT,  //returns total number of log lines which was filled in vectorList
	EJSON_SYSMGR_RPC_GET_LOG_LINE,   //returns log-message of a given index from vectorList
	EJSON_SYSMGR_RPC_GET_IP_ADDR,    //returns ip-addr of given "ethX" string
	EJSON_SYSMGR_RPC_SET_IP_ADDR,    //sets ip-addr of given "ethX" string
	EJSON_SYSMGR_RPC_GET_NETMASK,    //returns netmask of given "ethX" string
	EJSON_SYSMGR_RPC_SET_NETMASK,    //sets netmask of given "ethX" string
	//get device type
	EJSON_SYSMGR_RPC_RUN_SHELLCMD,    //runs a shell command
	EJSON_SYSMGR_RPC_DEVIDENT,
	EJSON_SYSMGR_RPC_EVNT_SUBSCRIBE, //custom event handler to re-subscribe events as per given event-config file(update subscription).
	EJSON_SYSMGR_RPC_DISP_CLEAR,     //clear display
	EJSON_SYSMGR_RPC_DISP_PRINT,     //print string on display
	EJSON_SYSMGR_RPC_DISP_GET_BKLT,  //read display backlight status
	EJSON_SYSMGR_RPC_DISP_SET_BKLT,  //control display backlight
	EJSON_SYSMGR_RPC_RUN_SHELLCMDTRIG, //runs a shell command but doesnt re-direct to /tmp/shellcmd.resp
	EJSON_SYSMGR_RPC_END,
	EJSON_SYSMGR_RPC_NONE
}EJSON_SYSMGR_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_MAC_ADDR
//EJSON_SYSMGR_RPC_SET_MAC_ADDR
#define SYSMGR_RPC_MAC_ADDR_GET             "get_mac_addr"
#define SYSMGR_RPC_MAC_ADDR_SET             "set_mac_addr"
#define SYSMGR_RPC_MAC_ADDR_ARG             "addr"
#define SYSMGR_RPC_MAC_ADDR_ARG_IFACE       "iface"
#define SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL  {"eth0","eth1","wlan0","br-lan","eth0.1","eth0.2","br0","tun1","enp110s0","unknown","none","\0"}
typedef enum EJSON_SYSMGR_IFACE_TYPE_T
{
	EJSON_SYSMGR_IFACE_TYPE_ETH0,
	EJSON_SYSMGR_IFACE_TYPE_ETH1,
	EJSON_SYSMGR_IFACE_TYPE_WLAN0,
	EJSON_SYSMGR_IFACE_TYPE_BRLAN,
	EJSON_SYSMGR_IFACE_TYPE_ETH01,
	EJSON_SYSMGR_IFACE_TYPE_ETH02,
	EJSON_SYSMGR_IFACE_TYPE_BR0,
	EJSON_SYSMGR_IFACE_TYPE_TUN1,
	EJSON_SYSMGR_IFACE_TYPE_ENP110S0,
	EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,
	EJSON_SYSMGR_IFACE_TYPE_NONE,
}EJSON_SYSMGR_IFACE_TYPE;
typedef struct SYSMGR_MAC_ADDR_PACKET_T //common communication object for different mac related rpc's
{
	EJSON_SYSMGR_IFACE_TYPE eth_type;
	char mac_addr[512];
	int EthCountIndx;//can be used as eth-index or eth-count
	char eth_name[512];
}SYSMGR_MAC_ADDR_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_ETH_COUNT
#define SYSMGR_RPC_ETH_COUNT_GET             "get_eth_count"
#define SYSMGR_RPC_ETH_COUNT_ARG             "count"
typedef SYSMGR_MAC_ADDR_PACKET SYSMGR_ETH_COUNT_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_ETH_NAME
#define SYSMGR_RPC_ETH_NAME_GET             "get_eth_name"
#define SYSMGR_RPC_ETH_NAME_ARG_INDEX       "index"
#define SYSMGR_RPC_ETH_NAME_ARG             "name"
typedef SYSMGR_MAC_ADDR_PACKET SYSMGR_ETH_NAME_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_LOADINFO
#define SYSMGR_RPC_LOADINFO_GET             "get_load_info"
#define SYSMGR_RPC_LOADINFO_ARG_CURRENT     "current"
#define SYSMGR_RPC_LOADINFO_ARG_AVERAGE     "average"
#define SYSMGR_RPC_LOADINFO_ARG_UPTIME      "uptime"
typedef struct SYSMGR_LOAD_INFO_PACKET_T
{
	char cur_load[512];
	char avg_load[512];
	char uptime[512];
}SYSMGR_LOAD_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_MEMINFO
#define SYSMGR_RPC_MEMINFO_GET             "get_mem_info"
#define SYSMGR_RPC_MEMINFO_ARG_MEM         "memMB"
#define SYSMGR_RPC_MEMINFO_ARG_MEMFREE     "memfreeMB"
#define SYSMGR_RPC_MEMINFO_ARG_MEMUSED     "memusedMB"
typedef struct SYSMGR_MEM_INFO_PACKET_T
{
	char mem[512];
	char memfree[512];
	char memused[512];
}SYSMGR_MEM_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_CPUINFO
#define SYSMGR_RPC_CPUINFO_GET             "get_cpu_info"
#define SYSMGR_RPC_CPUINFO_ARG_MODEL       "model"
#define SYSMGR_RPC_CPUINFO_ARG_CORES       "cores"
#define SYSMGR_RPC_CPUINFO_ARG_FREQ        "frequency"
typedef struct SYSMGR_CPU_INFO_PACKET_T
{
	char cpu_model[512];
	char cpu_cores[512];
	char cpu_freq[512];
}SYSMGR_CPU_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_DEV_OP (device operation)
//EJSON_SYSMGR_RPC_SET_DEV_OP (device operation)
#define SYSMGR_RPC_DEV_OP_GET             "get_device_operation"
#define SYSMGR_RPC_DEV_OP_SET             "set_device_operation"
#define SYSMGR_RPC_DEV_OP_ARG             "operation"
#define SYSMGR_RPC_DEV_OP_ARG_TABL        {"idle","on","laststate","standby","reboot","booting","rebooting","switchingoff","boot","unknown","none","\0"}
typedef enum EJSON_SYSMGR_DEV_OP_T
{
	EJSON_SYSMGR_DEV_OP_IDLE,        //IDLE
	EJSON_SYSMGR_DEV_OP_ON,          //ON
	EJSON_SYSMGR_DEV_OP_LAST_STATE,  //used for autostartup action
	EJSON_SYSMGR_DEV_OP_STANDBY,     //STANDBY(low power Wake-On-Lan mode)
	EJSON_SYSMGR_DEV_OP_REBOOT,      //RESTART
	EJSON_SYSMGR_DEV_OP_BOOTING,     //read_sts:STARTING
	EJSON_SYSMGR_DEV_OP_REBOOTING,   //read_sts:RESTARTING
	EJSON_SYSMGR_DEV_OP_SWTCHING_OFF,//read_sts:STOPPING
	EJSON_SYSMGR_DEV_OP_BOOT,        //START
	EJSON_SYSMGR_DEV_OP_UNKNOWN,
	EJSON_SYSMGR_DEV_OP_NONE
}EJSON_SYSMGR_DEV_OP;
typedef struct SYSMGR_DEV_OP_PACKET_T
{
	EJSON_SYSMGR_DEV_OP operation;
	char operation_str[255];//needed for settings store/restore(un-used for json-rpc mappers)
	int taskID;//return the taskID so that client can check the progress
}SYSMGR_DEV_OP_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_FMWVER,
#define SYSMGR_RPC_FMW_VER_GET             "get_fmw_version"
#define SYSMGR_RPC_FMW_VER_ARG_MODULE      "module"
#define SYSMGR_RPC_FMW_VER_ARG             "version"
#define SYSMGR_RPC_FMW_VER_ARG_TABL        {"current","backup","kernel","project","brbox1","brbox2","unknown","none","\0"}
typedef enum SYSMGR_FMW_MODULE_TYPE_T
{
	SYSMGR_FMW_MODULE_BRBOX_CURRENT,//current rootfs
	SYSMGR_FMW_MODULE_BRBOX_BACKUP, //backup rootfs
	SYSMGR_FMW_MODULE_BRBOX_KERNEL, //kernel version
	SYSMGR_FMW_MODULE_BRBOX_PROJECT,
	SYSMGR_FMW_MODULE_BRBOX1,
	SYSMGR_FMW_MODULE_BRBOX2,
	SYSMGR_FMW_MODULE_UNKNOWN,
	SYSMGR_FMW_MODULE_NONE
}SYSMGR_FMW_MODULE_TYPE;
typedef struct SYSMGR_FMW_MODULE_PACKET_T
{
	SYSMGR_FMW_MODULE_TYPE module;
	char cmn_fname_ver_str[512];//common for exchanging fmw_file_path or version string(MAX_FILE_PATH is 512bytes)
	int taskID;//for fmw update, return the taskID so that client can check the progress
}SYSMGR_FMW_MODULE_PACKET;
#define SYSMGR_BRDSK_TOOL         "brdskmgr"
#define SYSMGR_TEMP_FMW_READ_FILE "/tmp/temp-fmw-ver.txt"
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_BOOT_SYSTEM
#define SYSMGR_RPC_BOOT_SYSTEM_GET         "get_boot_system"
#define SYSMGR_RPC_BOOT_SYSTEM_ARG         "system"
#define SYSMGR_RPC_BOOT_SYSTEM_ARG_TABL    {"brbox1","brbox2","unknown","none","\0"}
typedef enum SYSMGR_BOOT_SYSTEM_TYPE_T
{
	SYSMGR_BOOT_SYSTEM_BRBOX1,
	SYSMGR_BOOT_SYSTEM_BRBOX2,
	SYSMGR_BOOT_SYSTEM_UNKNOWN,
	SYSMGR_BOOT_SYSTEM_NONE
}SYSMGR_BOOT_SYSTEM_TYPE;
typedef struct SYSMGR_BOOT_SYSTEM_PACKET_T
{
	SYSMGR_BOOT_SYSTEM_TYPE system;
}SYSMGR_BOOT_SYSTEM_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_SET_FMWUPDATE
#define SYSMGR_RPC_FMWUPDATE_SET             "firmware_update"
#define SYSMGR_RPC_FMWUPDATE_ARG             SYSMGR_RPC_FMW_VER_ARG_MODULE //"module"
#define SYSMGR_RPC_FMWUPDATE_ARG_TABL        SYSMGR_RPC_FMW_VER_ARG_TABL
#define SYSMGR_RPC_FMWUPDATE_ARG_FILEPATH    "filepath"
typedef SYSMGR_FMW_MODULE_PACKET SYSMGR_FMWUPDATE_PACKET;
#define SYSMGR_UPDATE_TOOL         "update"
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_SET_DOWNLOADFTP,
//EJSON_SYSMGR_RPC_SET_DOWNLOADTFTP,
#define SYSMGR_RPC_DOWNLOADFTP_SET           "download_ftp_file"
#define SYSMGR_RPC_DOWNLOADTFTP_SET          "download_tftp_file"
#define SYSMGR_RPC_DOWNLOADFILE_ARG_SRCADDR  "srcaddr" //incase of ftp it is the url, incase of tftp it is the serverip
#define SYSMGR_RPC_DOWNLOADFILE_ARG_SRCFILE  "srcfile"
#define SYSMGR_RPC_DOWNLOADFILE_ARG_TARFILE  "targetpath"
typedef struct SYSMGR_DOWNLOAD_FILE_PACKET_T
{
	char srcurl[1024];//source url of the ftp server's file
	char sourcefilepath[1024];//filepath of on the source file incase of tftp download
	char targetfilepath[1024];//filepath of on the target where file to be downloaded
	int taskID;//for file download, return the taskID so that client can check the progress
}SYSMGR_DOWNLOAD_FILE_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_ASYNCTASK //get-async-task-in-progress
#define SYSMGR_RPC_ASYNCTASK_GET         "get_async_task"
#define SYSMGR_RPC_ASYNCTASK_ARG         "task"
#define SYSMGR_RPC_ASYNCTASK_ARG_TABL    {"devop","fupdate","ftpdownload","tftpdownload","loglistupdt","shellcmd","devident","shellcmdtrig","none","none","\0"} //show unknown as none
typedef enum SYSMGR_ASYNCTASK_TYPE_T
{
	SYSMGR_ASYNCTASK_DEVOP,
	SYSMGR_ASYNCTASK_FUPDATE,
	SYSMGR_ASYNCTASK_FTPDOWNLOAD,
	SYSMGR_ASYNCTASK_TFTPDOWNLOAD,
	SYSMGR_ASYNCTASK_LOGLISTUPDATE,
	SYSMGR_ASYNCTASK_SHELLCMD,
	SYSMGR_ASYNCTASK_DEVIDENT,
	SYSMGR_ASYNCTASK_SHELLCMDTRIG,
	SYSMGR_ASYNCTASK_UNKNOWN,
	SYSMGR_ASYNCTASK_NONE
}SYSMGR_ASYNCTASK_TYPE;
typedef struct SYSMGR_ASYNCTASK_PACKET_T
{
	SYSMGR_ASYNCTASK_TYPE task;
}SYSMGR_ASYNCTASK_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_DEVTYPE
#define SYSMGR_RPC_DEVTYPE_GET         "get_devtype"
#define SYSMGR_RPC_DEVTYPE_ARG         "type"
#define SYSMGR_RPC_DEVTYPE_ARG_TABL    ADCMN_BOARD_TYPE_TABLE
typedef ADCMN_BOARD_TYPE SYSMGR_DEV_TYPE;
typedef struct SYSMGR_DEVTYPE_PACKET_T
{
	SYSMGR_DEV_TYPE DevType;
}SYSMGR_DEVTYPE_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_HOSTNAME,
//EJSON_SYSMGR_RPC_SET_HOSTNAME,
#define SYSMGR_RPC_HOSTNAME_GET        "get_hostname"
#define SYSMGR_RPC_HOSTNAME_SET        "set_hostname"
#define SYSMGR_RPC_HOSTNAME_ARG        "hostname"
#define HOST_NAME_FILE_PATH            "/mnt/settings/etc/hostname"
#define DEF_HOST_NAME_FILE_PATH        "/etc/hostname"
typedef struct SYSMGR_HOSTNAME_PACKET_T
{
	char hostname[1024];
}SYSMGR_HOSTNAME_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_MY_PUBLIC_IP
#define SYSMGR_RPC_MY_PUBLIC_IP_GET    "get_my_public_ip"
#define SYSMGR_RPC_MY_PUBLIC_IP_ARG    "ip"
typedef struct SYSMGR_MY_PUBLIC_IP_PACKET_T
{
	char ip[1024];
}SYSMGR_MY_PUBLIC_IP_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_SET_DEFAULT_HOSTNAME
#define SYSMGR_RPC_DEFAULT_HOSTNAME_SET    "set_default_hostname"
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_SET_UPDATE_LOG
//EJSON_SYSMGR_RPC_GET_LOG_COUNT
//EJSON_SYSMGR_RPC_GET_LOG_LINE
#define SYSMGR_RPC_UPDATE_LOG_SET    "update_loglist"
#define SYSMGR_RPC_LOG_COUNT_GET     "get_log_count"
#define SYSMGR_RPC_LOG_LINE_GET      "get_log_line"
#define SYSMGR_RPC_LOG_ARG_INDX      "index"
#define SYSMGR_RPC_LOG_ARG_LOGMSG    "logmsg"
#define SYSMGR_RPC_LOG_ARG_TOTAL     "logcount"
typedef struct SYSMGR_LOG_PACKET_T
{
	int index;
	int total;
	char logmsg[1024];
	int taskID;
}SYSMGR_LOG_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_IP_ADDR,    //returns ip-addr of given "ethX" string
//EJSON_SYSMGR_RPC_SET_IP_ADDR,    //sets ip-addr of given "ethX" string
//EJSON_SYSMGR_RPC_GET_NETMASK,    //returns netmask of given "ethX" string
//EJSON_SYSMGR_RPC_SET_NETMASK,    //sets netmask of given "ethX" string
#define SYSMGR_RPC_IP_ADDR_GET             "get_ip_addr"
#define SYSMGR_RPC_IP_ADDR_SET             "set_ip_addr"
#define SYSMGR_RPC_IP_ADDR_ARG             "addr"
#define SYSMGR_RPC_IP_ADDR_ARG_IFACE       SYSMGR_RPC_MAC_ADDR_ARG_IFACE
#define SYSMGR_RPC_IP_ADDR_ARG_IFACE_TABL  SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL
#define SYSMGR_RPC_NETMASK_GET             "get_netmask"
#define SYSMGR_RPC_NETMASK_SET             "set_netmask"
#define SYSMGR_RPC_NETMASK_ARG             "addr"
#define SYSMGR_RPC_NETMASK_ARG_IFACE       SYSMGR_RPC_MAC_ADDR_ARG_IFACE
#define SYSMGR_RPC_NETMASK_ARG_IFACE_TABL  SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL
typedef struct SYSMGR_NET_INFO_PACKET_T //common communication object for netinfo related rpc's
{
	EJSON_SYSMGR_IFACE_TYPE eth_type;
	char addr[512];
	char eth_name[512];
}SYSMGR_NET_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_RUN_SHELLCMD
//EJSON_SYSMGR_RPC_RUN_SHELLCMDTRIG
#define SYSMGR_RPC_RUN_SHELLCMD      "run_shellcmd"
#define SYSMGR_RPC_RUN_SHELLCMDTRIG  "run_shellcmdtrig"
#define SYSMGR_RPC_SHELLCMD_ARG_CMD  "command"
typedef struct SYSMGR_SHELLCMD_PACKET_T
{
	char cmd[1024];
	int taskID;
}SYSMGR_SHELLCMD_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_DEVIDENT
#define SYSMGR_RPC_DEVIDENT  "device_identify"
typedef struct SYSMGR_DEVIDENT_PACKET_T
{
	//char cmd[1024];
	int taskID;
}SYSMGR_DEVIDENT_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_EVNT_SUBSCRIBE
#define SYSMGR_RPC_EVNT_SUBSCRIBE    "re_subscribe_events"
typedef struct SYSMGR_EVNT_SUBSCR_PACKET_T
{
	//char cmd[1024];
	int taskID;
}SYSMGR_EVNT_SUBSCR_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_DISP_CLEAR,
//EJSON_SYSMGR_RPC_DISP_PRINT,
#define	SYSMGR_RPC_DISP_CLEAR           "display_clear"
#define	SYSMGR_RPC_DISP_PRINT           "display_print"
#define SYSMGR_RPC_DISP_PRINT_LINE_ARG  "line"
#define SYSMGR_RPC_DISP_PRINT_MESG_ARG  "msg"
#define SYSMGR_RPC_DISP_LINE_ARG_TABL    {"line1","line2","line3","line4","lineall","none","none","\0"}
//important: keep enum DISPLAY_LINE and EJSON_DISPSRV_LINE same
typedef enum EJSON_SYSMGR_LINE_T
{
	EJSON_SYSMGR_LINE_1,
	EJSON_SYSMGR_LINE_2,
	EJSON_SYSMGR_LINE_3,
	EJSON_SYSMGR_LINE_4,
	EJSON_SYSMGR_LINE_FULL,
	EJSON_SYSMGR_LINE_UNKNOWN,
	EJSON_SYSMGR_LINE_NONE
}EJSON_SYSMGR_LINE;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_DISP_GET_BKLT,
//EJSON_SYSMGR_RPC_DISP_SET_BKLT,
#define	SYSMGR_RPC_DISP_GET_BKLT        "display_backlight_get"
#define	SYSMGR_RPC_DISP_SET_BKLT        "display_backlight_set"
#define SYSMGR_RPC_DISP_BKLT_ARG        "status"
#define SYSMGR_RPC_DISP_BKLT_ARG_TABL   {"off","on","unknown","none","\0"}
typedef enum SYSMGR_BKLT_STS_T
{
	SYSMGR_BKLT_STS_OFF,
	SYSMGR_BKLT_STS_ON,
	SYSMGR_BKLT_STS_UNKNOWN,
	SYSMGR_BKLT_STS_NONE
}SYSMGR_BKLT_STS;
typedef struct SYSMGR_PRINT_PACKET_T
{
	EJSON_SYSMGR_LINE line;
	char msg[1024];
	SYSMGR_BKLT_STS bklsts;//backlight status
}SYSMGR_PRINT_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to smart-eye-service here
typedef struct SYSMGR_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	//std::string StrTmp;//pChar //Char //Int //pInt //Float //Enum
	SYSMGR_DEV_OP_PACKET DevOp;
	char crnt_fmwver[100];//current version
	char bkup_fmwver[100];//backup version
	char krnl_fmwver[100];//kernel version
	SYSMGR_BOOT_SYSTEM_TYPE bootsys;//brbox1/brbox2
	EJSON_SYSMGR_RPC_TYPES AsyncCmdInProgress;
	void *pLogger;//log message handler
	void *pDevIdent;//device identify handler interface
	std::string EvntMonitorConfigFile;
	void *pEventDefault;
	void *pEventCustom;
	void *pDisplay;
	SYSMGR_CMN_DATA_CACHE_T()
	{
		AsyncCmdInProgress=EJSON_SYSMGR_RPC_NONE;
		pLogger=NULL;
		pDevIdent=NULL;
		EvntMonitorConfigFile="";
		pEventDefault=NULL;
		pEventCustom=NULL;
	};//initialize variables here
	~ SYSMGR_CMN_DATA_CACHE_T(){};
}SYSMGR_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */
#endif
