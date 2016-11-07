/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ROUTERLAYER_H
#define ROUTERLAYER_H
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "map"
#include "ns3/epc-enb-application.h"
namespace ns3 {

/* ... */
class RouterLayer {
        public:
        RouterLayer();
        ~RouterLayer();
        void senddownlink(Ptr<Packet> packet,uint16_t protocolNumber);
        void SendDownlink(Ptr<Packet> packet,uint16_t protocolNumber,Ptr<NetDevice> epc_enb_wifi_netdevice);
        void mapfunc(Mac48Address macaddr,Ipv4Address ipv4address,uint16_t enb_id);
        void senduplink(Ptr<Packet> p,Ptr<NetDevice> m_device,uint16_t protocolNumber);
        void forwardtolteepcenb(Ptr<Packet> p,uint32_t teid);
        void forwardtoltepdcp(Ptr<Packet> p,uint32_t teid);
        void send_to_ue_lte_rrc(Ptr<Packet> p);
        static std::map<uint16_t, Ptr<NetDevice> > epc_enb_wifi_netdevice;
        static std::map<Ipv4Address,uint16_t> mapenbtowifi;
        static Ptr<EpcEnbApplication> routerlayerepcenbapp_object;
       static std::map<Ipv4Address,Mac48Address> mapaddress;
       static std::map<uint16_t,Mac48Address>wifi_Ap_macaddr;
       static bool multiple_enB;
};
}

#endif /* ROUTERLAYER_H */

