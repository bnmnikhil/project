/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "routerlayer.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4.h"
#include "ns3/epc-enb-application.h"
#include "ns3/lte-pdcp-header.h"
//#include "ns3/common.h"
namespace ns3 {

/* ... */
bool RouterLayer::multiple_enB=0;
std::map<uint16_t,Ptr<NetDevice> > RouterLayer::epc_enb_wifi_netdevice;
std::map<uint16_t,Mac48Address> RouterLayer::wifi_Ap_macaddr;
std::map<Ipv4Address,Mac48Address> RouterLayer::mapaddress;
std::map<Ipv4Address,uint16_t> RouterLayer::mapenbtowifi;
Ptr<EpcEnbApplication> RouterLayer::routerlayerepcenbapp_object;
RouterLayer::RouterLayer() {
	//NS_LOG_FUNCTION(this);
}

RouterLayer::~RouterLayer() {
	//NS_LOG_FUNCTION(this);
}

void RouterLayer::senddownlink(Ptr<Packet> packet,uint16_t protocolNumber) {
	//NS_LOG_FUNCTION(this);
	//packet->Print(std::cout);
	Ipv4Header ipv4Header;
	packet->PeekHeader(ipv4Header);
	// std::cout<<"Packet IP" <<ipv4Header.GetDestination()<<std::endl;
	Mac48Address mac=mapaddress.find(ipv4Header.GetDestination())->second;
	uint16_t enb_wifi_net_device_number=mapenbtowifi.find(ipv4Header.GetDestination())->second;
	//std::cout<<"MAC Address  = "<<mac<<std::endl;
	//packet->Print(std::cout);
	// std::cout<<" enb_wifi_net_device_number =" <<mapenbtowifi.find(ipv4Header.GetDestination())->first <<" second " <<mapenbtowifi.find(ipv4Header.GetDestination())->second <<std::endl;
	epc_enb_wifi_netdevice[enb_wifi_net_device_number]->Send(packet,mac,2048);
}
/*
void RouterLayer::SendDownlink(Ptr<Packet> packet,uint16_t protocolNumber,Ptr<NetDevice> epc_enb_wifi_netdevice) {
	//NS_LOG_FUNCTION(this);
	//packet->Print(std::cout);
	Ipv4Header ipv4Header;
	packet->PeekHeader(ipv4Header);
	Mac48Address mac=mapaddress.find(ipv4Header.GetDestination())->second;
	//std::cout<<"MAC Address  = "<<mac<<std::endl;
	//packet->Print(std::cout);
	epc_enb_wifi_netdevice->Send(packet,mac,2048);
}
*/
void RouterLayer::mapfunc(Mac48Address macaddr,Ipv4Address ipv4address, uint16_t enb) {
	mapaddress[ipv4address]=macaddr;
	mapenbtowifi[ipv4address]=enb;
	ns3::EpcEnbApplication::macipmap[macaddr]=ipv4address;
}
void RouterLayer::senduplink(Ptr<Packet> p,Ptr<NetDevice> m_device,uint16_t protocolNumber) {
	m_device->Send(p,wifi_Ap_macaddr[0],protocolNumber);
}
void RouterLayer::forwardtolteepcenb(Ptr<Packet> p,uint32_t teid) {
	EpcEnbApplication::uplinkthrpt_wifi+=p->GetSize();
	routerlayerepcenbapp_object->SendToS1uSocket(p,teid);
}

void RouterLayer::send_to_ue_lte_rrc(Ptr<Packet> p) {
//	EpcEnbApplication::uplinkthrpt_wifi+=p->GetSize();
//	routerlayerepcenbapp_object->SendToS1uSocket(p);
}


/*
#ifdef BelowPDCP
void RouterLayer::senddownlink(Ptr<Packet> packet,uint16_t protocolNumber) {
	//NS_LOG_FUNCTION(this);
	std::cout<<"Printing definition = "<<BelowPDCP<<std::endl;
	Ipv4Header ipv4Header;
	LtePdcpHeader pdcpHeader;
	packet->RemoveHeader(pdcpHeader);
	packet->PeekHeader(ipv4Header);
	Mac48Address mac=mapaddress.find(ipv4Header.GetDestination())->second;
	packet->AddHeader(pdcpHeader);
	epc_enb_wifi_netdevice[0]->Send(packet,mac,2048);
}
void RouterLayer::mapfunc(Mac48Address macaddr,Ipv4Address ipv4address) {
	mapaddress[ipv4address]=macaddr;
	ns3::EpcEnbApplication::macipmap[macaddr]=ipv4address;
}
void RouterLayer::senduplink(Ptr<Packet> p,Ptr<NetDevice> m_device,uint16_t protocolNumber) {
	m_device->Send(p,wifi_Ap_macaddr[0],protocolNumber);
}
void RouterLayer::forwardtoltepdcp(Ptr<Packet> p,uint32_t teid) {
	EpcEnbApplication::uplinkthrpt_wifi+=p->GetSize();
	routerlayerepcenbapp_object->SendToS1uSocket(p,teid);
}
#endif
*/
}

