/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ipv4-end-point.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-flow-probe.h"
#include "ns3/epc-enb-application.h"
NS_LOG_COMPONENT_DEFINE ("Ipv4EndPoint");

namespace ns3 {/*
class Ipv4FlowProbeTag : public Tag
{
public:
  *
   * \brief Get the type ID.
   * \return the object TypeId

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buf) const;
  virtual void Deserialize (TagBuffer buf);
  virtual void Print (std::ostream &os) const;
  Ipv4FlowProbeTag ();
  *
   * \brief Consructor
   * \param flowId the flow identifier
   * \param packetId the packet identifier
   * \param packetSize the packet size

  Ipv4FlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize);
  *
   * \brief Set the flow identifier
   * \param flowId the flow identifier

  void SetFlowId (uint32_t flowId);
  *
   * \brief Set the packet identifier
   * \param packetId the packet identifier

  void SetPacketId (uint32_t packetId);
  *
   * \brief Set the packet size
   * \param packetSize the packet size

  void SetPacketSize (uint32_t packetSize);
  *
   * \brief Set the flow identifier
   * \returns the flow identifier

  uint32_t GetFlowId (void) const;
  *
   * \brief Set the packet identifier
   * \returns the packet identifier

  uint32_t GetPacketId (void) const;
  *
   * \brief Get the packet size
   * \returns the packet size

  uint32_t GetPacketSize (void) const;
private:
  uint32_t m_flowId;      //!< flow identifier
  uint32_t m_packetId;    //!< packet identifier
  uint32_t m_packetSize;  //!< packet size

};

TypeId
Ipv4FlowProbeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4FlowProbeTag")
    .SetParent<Tag> ()
    .AddConstructor<Ipv4FlowProbeTag> ()
  ;
  return tid;
}
TypeId
Ipv4FlowProbeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
Ipv4FlowProbeTag::GetSerializedSize (void) const
{
  return 4 + 4 + 4;
}
void
Ipv4FlowProbeTag::Serialize (TagBuffer buf) const
{
  buf.WriteU32 (m_flowId);
  buf.WriteU32 (m_packetId);
  buf.WriteU32 (m_packetSize);
}
void
Ipv4FlowProbeTag::Deserialize (TagBuffer buf)
{
  m_flowId = buf.ReadU32 ();
  m_packetId = buf.ReadU32 ();
  m_packetSize = buf.ReadU32 ();
}
void
Ipv4FlowProbeTag::Print (std::ostream &os) const
{
  os << "FlowId=" << m_flowId;
  os << "PacketId=" << m_packetId;
  os << "PacketSize=" << m_packetSize;
}
Ipv4FlowProbeTag::Ipv4FlowProbeTag ()
  : Tag ()
{
}

Ipv4FlowProbeTag::Ipv4FlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize)
  : Tag (), m_flowId (flowId), m_packetId (packetId), m_packetSize (packetSize)
{
}

void
Ipv4FlowProbeTag::SetFlowId (uint32_t id)
{
  m_flowId = id;
}
void
Ipv4FlowProbeTag::SetPacketId (uint32_t id)
{
  m_packetId = id;
}
void
Ipv4FlowProbeTag::SetPacketSize (uint32_t size)
{
  m_packetSize = size;
}
uint32_t
Ipv4FlowProbeTag::GetFlowId (void) const
{
  return m_flowId;
}
uint32_t
Ipv4FlowProbeTag::GetPacketId (void) const
{
  return m_packetId;
}
uint32_t
Ipv4FlowProbeTag::GetPacketSize (void) const
{
  return m_packetSize;
}
*/
Ipv4EndPoint::Ipv4EndPoint (Ipv4Address address, uint16_t port)
  : m_localAddr (address), 
    m_localPort (port),
    m_peerAddr (Ipv4Address::GetAny ()),
    m_peerPort (0)
{
  NS_LOG_FUNCTION (this << address << port);
}
Ipv4EndPoint::~Ipv4EndPoint ()
{
  NS_LOG_FUNCTION (this);
  if (!m_destroyCallback.IsNull ())
    {
      m_destroyCallback ();
    }
  m_rxCallback.Nullify ();
  m_icmpCallback.Nullify ();
  m_destroyCallback.Nullify ();
}

Ipv4Address 
Ipv4EndPoint::GetLocalAddress (void)
{
  NS_LOG_FUNCTION (this);
  return m_localAddr;
}

void 
Ipv4EndPoint::SetLocalAddress (Ipv4Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_localAddr = address;
}

uint16_t 
Ipv4EndPoint::GetLocalPort (void)
{
  NS_LOG_FUNCTION (this);
  return m_localPort;
}
Ipv4Address 
Ipv4EndPoint::GetPeerAddress (void)
{
  NS_LOG_FUNCTION (this);
  return m_peerAddr;
}
uint16_t 
Ipv4EndPoint::GetPeerPort (void)
{
  NS_LOG_FUNCTION (this);
  return m_peerPort;
}
void 
Ipv4EndPoint::SetPeer (Ipv4Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  m_peerAddr = address;
  m_peerPort = port;
}

void
Ipv4EndPoint::BindToNetDevice (Ptr<NetDevice> netdevice)
{
  NS_LOG_FUNCTION (this << netdevice);
  m_boundnetdevice = netdevice;
  return;
}

Ptr<NetDevice> 
Ipv4EndPoint::GetBoundNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  return m_boundnetdevice;
}

void 
Ipv4EndPoint::SetRxCallback (Callback<void,Ptr<Packet>, Ipv4Header, uint16_t, Ptr<Ipv4Interface> > callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_rxCallback = callback;
}
void 
Ipv4EndPoint::SetIcmpCallback (Callback<void,Ipv4Address,uint8_t,uint8_t,uint8_t,uint32_t> callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_icmpCallback = callback;
}

void 
Ipv4EndPoint::SetDestroyCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_destroyCallback = callback;
}

void 
Ipv4EndPoint::ForwardUp (Ptr<Packet> p, const Ipv4Header& header, uint16_t sport,
                         Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << p << &header << sport << incomingInterface);

 /* EpcEnbApplication epcenbapp;
   Callback<void, uint32_t,uint32_t, double, double> cb;
   cb = MakeCallback(&EpcEnbApplication::udpstats, &epcenbapp);
   Ptr<Packet> pkt=p->Copy();
   Ipv4Header ipv4header;
   pkt->RemoveHeader(ipv4header);


  // Ipv4FlowClassifier pktclassifier;
   uint32_t fid,pid;
  // pktclassifier.Classify(ipv4header,p,&fid,&pid);
   Ipv4FlowProbeTag flowprobe;
    bool found;
    found =pkt->PeekPacketTag(flowprobe);
    if(found) {
  	  fid=flowprobe.GetFlowId();
  	  pid=flowprobe.GetPacketId();
	std::cout << "*************receive**********************************\n";
    	  p->Print(std::cout);
    	  std::cout << std::endl;

    }
   pkt->AddHeader(ipv4header);
   if(found) {
 	  cb(fid, pid, p->GetSize(), Simulator::Now ().GetSeconds());
   }*/
  if (!m_rxCallback.IsNull ())
    {
      Simulator::ScheduleNow (&Ipv4EndPoint::DoForwardUp, this, p, header, sport, 
                              incomingInterface);
    }
}
void 
Ipv4EndPoint::DoForwardUp (Ptr<Packet> p, const Ipv4Header& header, uint16_t sport,
                           Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << p << &header << sport << incomingInterface);

  if (!m_rxCallback.IsNull ())
    {
      m_rxCallback (p, header, sport, incomingInterface);
    }
}

void 
Ipv4EndPoint::ForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                           uint8_t icmpType, uint8_t icmpCode,
                           uint32_t icmpInfo)
{
  NS_LOG_FUNCTION (this << icmpSource << (uint32_t)icmpTtl << (uint32_t)icmpType <<
                   (uint32_t)icmpCode << icmpInfo);
  if (!m_icmpCallback.IsNull ())
    {
      Simulator::ScheduleNow (&Ipv4EndPoint::DoForwardIcmp, this, 
                              icmpSource, icmpTtl, icmpType, icmpCode, icmpInfo);
    }
}
void 
Ipv4EndPoint::DoForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                             uint8_t icmpType, uint8_t icmpCode,
                             uint32_t icmpInfo)
{
  NS_LOG_FUNCTION (this << icmpSource << static_cast<uint32_t> (icmpTtl) << static_cast<uint32_t> (icmpType) << static_cast<uint32_t> (icmpCode) << icmpInfo);
  
  if (!m_icmpCallback.IsNull ())
    {
      m_icmpCallback (icmpSource,icmpTtl,icmpType,icmpCode,icmpInfo);
    }
}

} // namespace ns3
