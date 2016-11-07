/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "routerlayer.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4.h"
#include "ns3/epc-enb-application.h"
namespace ns3 {

/* ... */
bool RouterLayer::multiple_enB=0;
std::map<uint16_t,Ptr<NetDevice> > RouterLayer::epc_enb_wifi_netdevice;
std::map<uint16_t,Mac48Address> RouterLayer::wifi_Ap_macaddr;
std::map<Mac48Address,Mac48Address> RouterLayer::ueenbmap;
std::map<Ipv4Address,Mac48Address> RouterLayer::mapaddress;
Ptr<EpcEnbApplication> RouterLayer::routerlayerepcenbapp_object;
RouterLayer::RouterLayer() {
	//NS_LOG_FUNCTION(this);
}

RouterLayer::~RouterLayer() {
	//NS_LOG_FUNCTION(this);
}
void RouterLayer::senddownlink(Ptr<Packet> packet,uint16_t protocolNumber) {
	//NS_LOG_FUNCTION(this);
       
	Ipv4Header ipv4Header;
	packet->PeekHeader(ipv4Header);
	Mac48Address mac=mapaddress.find(ipv4Header.GetDestination())->second;
         for (uint16_t i = 0; i <7 ; i++)
	{
                if(ueenbmap[mac]==wifi_Ap_macaddr[i])
	          epc_enb_wifi_netdevice[i]->Send(packet,mac,2048);
        }
}
void RouterLayer::mapfunc(Mac48Address macaddr,Ipv4Address ipv4address) {
        
	mapaddress[ipv4address]=macaddr;
	ns3::EpcEnbApplication::macipmap[macaddr]=ipv4address;
}
void RouterLayer::senduplink(Ptr<Packet> p,Ptr<NetDevice> m_device,uint16_t protocolNumber) {
        
	Ipv4Header ipv4Header;
	p->PeekHeader(ipv4Header);

        Mac48Address mac= mapaddress[ipv4Header.GetSource()];
	
	m_device->Send(p,ueenbmap[mac],protocolNumber);
        
}
void RouterLayer::forwardtolteepcenb(Ptr<Packet> p,uint32_t teid) {
	EpcEnbApplication::uplinkthrpt_wifi+=p->GetSize();
	routerlayerepcenbapp_object->SendToS1uSocket(p,teid);
}
}

