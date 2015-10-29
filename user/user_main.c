#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "espconn.h"
#include "ip_addr.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
struct espconn conn1;
esp_tcp tcp1;

struct espconn threatbutt_conn;
ip_addr_t threatbutt_ip;
esp_tcp threatbutt_tcp;

os_timer_t myTimer;

char threatbutt_host[] = "api.threatbutt.io";
char threatbutt_path[] = "/api"; 
char attackip[256];
char buffer[ 2048 ];

char attacker_buf[256];
char vector_buf[256];

//list of country codes
const char *countries[] = {"kaz", "afg", "ala", "alb", "dza", "asm", "alb", "and", "aia", "ata", "atg",
"arg", "arm", "abw", "aus", "aut", "aze", "bhs", "bhr", "bgd", "brb",
"blr", "bel", "blz", "ben", "bmu", "btn", "bol", "bih", "bwa", "bvt",
"bra", "vgb", "iot", "brn", "bgr", "bfa", "bdi", "khm", "cmr", "can",
"cpv", "cym", "caf", "tcd", "chl", "chn", "hkg", "mac", "cxr", "cck",
"col", "com", "cog", "cod", "cok", "cri", "civ", "hrv", "cub", "cyp",
"cze", "dnk", "dji", "dma", "dom", "ecu", "egy", "slv", "gnq", "eri",
"est", "eth", "flk", "fro", "fji", "fin", "fra", "guf", "pyf", "atf",
"gab", "gmb", "geo", "deu", "gha", "gib", "grc", "grl", "grd", "glp",
"gum", "gtm", "ggy", "gin", "gnb", "guy", "hti", "hmd", "vat", "hnd",
"hun", "isl", "ind", "idn", "irn", "irq", "irl", "imn", "isr", "ita",
"jam", "jpn", "jey", "jor", "kaz", "ken", "kir", "prk", "kor", "kwt",
"kgz", "lao", "lva", "lbn", "lso", "lbr", "lby", "lie", "ltu", "lux",
"mkd", "mdg", "mwi", "mys", "mdv", "mli", "mlt", "mhl", "mtq", "mrt",
"mus", "myt", "mex", "fsm", "mda", "mco", "mng", "mne", "msr", "mar",
"moz", "mmr", "nam", "nru", "npl", "nld", "ant", "ncl", "nzl", "nic",
"ner", "nga", "niu", "nfk", "mnp", "nor", "omn", "pak", "plw", "pse",
"pan", "png", "pry", "per", "phl", "pcn", "pol", "prt", "pri", "qat",
"reu", "rou", "rus", "rwa", "blm", "shn", "kna", "lca", "maf", "spm",
"vct", "wsm", "smr", "stp", "sau", "sen", "srb", "syc", "sle", "sgp",
"svk", "svn", "slb", "som", "zaf", "sgs", "ssd", "esp", "lka", "sdn",
"sur", "sjm", "swz", "swe", "che", "syr", "twn", "tjk", "tza", "tha",
"tls", "tgo", "tkl", "ton", "tto", "tun", "tur", "tkm", "tca", "tuv",
"uga", "ukr", "are", "gbr", "usa", "umi", "ury", "uzb", "vut", "ven", 
"vnm", "vir", "wlf", "esh", "yem", "zmb", "zwe"
};
#define n_countries (sizeof (countries) / sizeof (const char *))

const char *vector_names[] = {
"Tor",
"SSL certificate",
"Bad BIOS",
"4Chan",
"FLAMMABLE 'DATA FUEL'",
"Morris Worm",
"BUTTER PANDA",
"WordPress plugins",
"0x41414141",
"lil' Bobby Tables",
"Slammer",
"marketing has yet to get back to us",
"Advanced Persistent Bad Day",
"Heartbleed",
"Rowhammer",
"StageFright",
"UNEXPECTED LITIGATION",
"EULA Violation",
"Netbus",
"Samy is my hero",
"A RUNAWAY HACKED JEEP!",
"@mdowd blog post",
"netcat",
"metasploit tutorial",
"stoned",
"melissa",
"stuxnet",
"V.E.N.O.M.",
"QUANTUM LEAP",
"Back Orifice",
"TROUSERSNAKE",
"Flame",
"Edward Snowden",
"Julian Assange",
"do you even lift bro?",
"default password",
"SJW",
"angry yelp review",
"Goatse",
"Tubgirl",
"Captain Crunch whistle",
"PHRACK article",
"stolen from Stefen Esser"
"404 NOT FOUND",
"SEG FAULT",
};
#define n_vector_names (sizeof (vector_names) / sizeof (const char *))

const char *responses[] = {
    "UPGRADE TO PREMIUM FOR MORE INFO",
    "it's probably fine \\o<",
    "NSA told us,we can't tell you",
    "it didn't work good",
    "We'll just call it a \"glitch\"",
    "IT'S CYBER POMPEII !",
    "IT'S SUPER EFFECTIVE!",
    "check yo' centrifuges"
    };
#define n_responses (sizeof (responses) / sizeof (const char *))

        
 static void loop(os_event_t *events);

bool attacker_response;
bool vector_response;



//generates and sends the attack response
void send_response(){
    char response[512];
    const char *attacker_country = countries[os_random()%n_countries];
    const char *vector = vector_names[os_random()%n_vector_names];
    const char *response_str = responses[os_random()%n_responses];
    os_sprintf(response,"{%s(%s);uses %s;%s}",attacker_country,attackip,vector, response_str);  
    uart1_tx_buffer(response,strlen(response));
}

//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events)
{
    
    system_os_post(user_procTaskPrio, 0, 0 );
}
void timerCallback(void *pArg){
    struct station_info *stationInfo = wifi_softap_get_station_info();
    if(stationInfo != NULL){
        while(stationInfo != NULL) { 
        os_printf("%d.%d.%d.%d", IP2STR(&(stationInfo->ip))); 
        os_sprintf(attackip, "%d.%d.%d.%d", IP2STR(&(stationInfo->ip)));
        send_response();            
        stationInfo = STAILQ_NEXT(stationInfo, next);
    }
    wifi_softap_free_station_info();
    }
}


void vector_received( void *arg, char *pdata, unsigned short len )
{
    struct espconn *conn = arg;
    os_printf( "%s: %s\n", __FUNCTION__, pdata );
    //scan through response and get output 
    if(len > 256)
       return; 
    char response[512];
    int i=0; 
    int j=1;
    int z=0;  
    for(i=0; i<len; i++){
        if(pdata[i] == '['){
            while(pdata[i+j] != ']'){
                vector_buf[z++] = pdata[i+j];
                j++;
            }
            vector_buf[z+1] = 0; 
        }
    }
    //espconn_regist_recvcb( conn, create_response);
    //os_delay_us(1000000); //wait a second before sending next request 
    os_sprintf(response,"{%s;uses %s;%s}",attackip,attacker_buf, vector_buf);  
    uart1_tx_buffer(response,strlen(response)); 
    os_memset(response,0x00, sizeof(response));
    os_memset(attacker_buf,0x00,sizeof(attacker_buf));
    os_memset(vector_buf,0x00,sizeof(vector_buf));
    espconn_disconnect( conn );
}

void attack_received( void *arg, char *pdata, unsigned short len )
{
    struct espconn *conn = arg;
    os_printf( "%s: %s\n", __FUNCTION__, pdata );
    if(len > 256)
       return; 
    //scan through response and get output 
    int i=0; 
    int j=1;
    int z=0;
    for(i=0; i<len; i++){
        if(pdata[i] == '['){
            while(pdata[i+j] != ']'){
                attacker_buf[z++] = pdata[i+j];
                j++;
            }
        attacker_buf[z+1] = 0;  //zero truncate string
        }
    }
    
    //send vector request 
    espconn_regist_recvcb( conn, vector_received );
    os_sprintf( buffer, "POST %s/md5/5eb63bbbe01eeed093cb22bb8f5acdc3 HTTP/1.1\r\nUser-Agent:Threatbutt IoT Platform\r\nHost: %s\r\nAccept: */*\r\nProxy-Connection: Keep-Alive\r\nContent-Length: 0\r\n\r\n",threatbutt_path, threatbutt_host);
    os_printf( "Sending: vector %s\n", buffer );
    espconn_sent( conn, buffer, os_strlen( buffer ));
}



void tcp_connected( void *arg )
{
    struct espconn *conn = arg;
    
    os_printf( "%s\n", __FUNCTION__ );
    //espconn_regist_recvcb( conn, attack_received );
    
    os_sprintf( buffer, "POST %s HTTP/1.1\r\nUser-Agent: Threatbutt IoT Platform\r\nHost: %s\r\nAccept: */*\r\nProxy-Connection: Keep-Alive\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n%s",threatbutt_path, threatbutt_host,os_strlen(attackip), attackip);
    //attacker_response = true;
    os_printf( "Sending attack: %s\n", buffer );
    espconn_sent( conn, buffer, os_strlen( buffer ));
    
    
}

void dns_done( const char *name, ip_addr_t *ipaddr, void *arg )
{
    struct espconn *conn = arg;
    os_printf( "%s\n", __FUNCTION__ );

    if ( ipaddr == NULL)
    {
        os_printf("DNS lookup failed\n");
        wifi_station_disconnect();
    }
    else
    {
        os_printf("Connecting...\n" );
        
        conn->type = ESPCONN_TCP;
        conn->state = ESPCONN_NONE;
        conn->proto.tcp=&threatbutt_tcp;
        conn->proto.tcp->local_port = espconn_port();
        conn->proto.tcp->remote_port = 1234;
        os_memcpy(conn->proto.tcp->remote_ip, &ipaddr->addr, 4 );
        
        espconn_regist_connectcb( conn, tcp_connected );
        //espconn_regist_disconcb( conn, tcp_disconnected );
        
        espconn_connect( conn );
    }
}

void ICACHE_FLASH_ATTR
 connectCB(void *arg) {
    //struct espconn *pNewEspConn = (struct espconn *)arg;
    os_printf("Hello\n\r");
}


LOCAL void ICACHE_FLASH_ATTR initDone() {
    wifi_set_opmode_current(SOFTAP_MODE);
    struct softap_config config; 
    os_strcpy(config.ssid, "Free WiFi - Coursety @threatbutt");
    os_strcpy(config.password, "");
    config.ssid_len = 0;
    config.authmode = AUTH_OPEN;
    config.ssid_hidden = 0;
    config.max_connection = 4;
    wifi_softap_set_config_current(&config);
    wifi_softap_dhcps_start();
    uint8 stationCount = wifi_softap_get_station_num(); 
    struct station_info currentstation;


} // End of initDone

LOCAL void ICACHE_FLASH_ATTR recvCB(void *arg) {
    struct espconn *pEspConn = (struct espconn *)arg;
    os_printf("Received IP!! = %d.%d.%d.%d\n", IP2STR(pEspConn->proto.tcp->remote_ip));
    os_sprintf(attackip, "%d.%d.%d.%d", IP2STR(pEspConn->proto.tcp->remote_ip) );
    send_response();    
    //espconn_gethostbyname( &threatbutt_conn, threatbutt_host, &threatbutt_ip, dns_done );
} // End of recvCB


LOCAL void ICACHE_FLASH_ATTR setupTCP() {
    conn1.type = ESPCONN_TCP;
    conn1.state = ESPCONN_NONE;
    tcp1.local_port = 8080;
    conn1.proto.tcp = &tcp1;
    espconn_accept(&conn1); 
    espconn_create(&conn1);
    espconn_regist_connectcb(&conn1, recvCB); 
    
    //espconn_regist_recvcb(&conn1, recvCB);
    os_printf("Listening for data\n");
} // End of setupUDP

LOCAL void ICACHE_FLASH_ATTR eventCB(System_Event_t *event) {
        switch(event->event) {
            case EVENT_STAMODE_CONNECTED:
                os_printf("Event: EVENT_STAMODE_CONNECTED");
                break;
            case EVENT_STAMODE_DISCONNECTED:
                os_printf("Event: EVENT_STAMODE_DISCONNECTED");
                break;
            case EVENT_STAMODE_AUTHMODE_CHANGE:
                os_printf("Event: EVENT_STAMODE_AUTHMODE_CHANGE");
                break;
            case EVENT_STAMODE_GOT_IP:
                os_printf("Event: EVENT_STAMODE_GOT_IP");
                //os_printf("IP: %d.%d.%d.%d\n", IP2STR(&event->event_info.got_ip.ip));
                //setupTCP();
                break;
            case EVENT_SOFTAPMODE_STACONNECTED:{
                struct station_info *stationInfo = wifi_softap_get_station_info();
                if(stationInfo != NULL){
                    os_printf("%d.%d.%d.%d", IP2STR(&(stationInfo->ip))); 
                    os_sprintf(attackip, "%d.%d.%d.%d", IP2STR(&(stationInfo->ip)));
                    send_response();            
                }
                wifi_softap_free_station_info(); 
                os_printf("Event: EVENT_SOFTAPMODE_STACONNECTED");
                }
                break;
            case EVENT_SOFTAPMODE_STADISCONNECTED:
                os_printf("Event: EVENT_SOFTAPMODE_STADISCONNECTED");
                break;
            case EVENT_SOFTAPMODE_PROBEREQRECVED:
                //os_printf("Event: EVENT_PROBEREQUEST");
                break;
            default:
                os_printf("Unexpected event: %d\r\n", event->event);
                break;
        }
} // End of eventCB

void ICACHE_FLASH_ATTR user_init()
{
    uart_init(BIT_RATE_9600, BIT_RATE_9600);
    wifi_station_set_hostname( "Threatbutt IoT Protector" ); 
    system_init_done_cb(initDone);
      
    os_timer_setfn(&myTimer, timerCallback, NULL);
    os_timer_arm(&myTimer, 10000, 1); 
}
