/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jaume.nin@cttc.cat>
 */
#include "ns3/energy-module.h"
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/epc-enb-application.h"
#include "ns3/routerlayer.h"
#include "map"
#include "ns3/lte-ue-mac.h"
#include "sstream"
#include "time.h"
#include "ns3/ipv4-interface.h"
//#include "ns3/ltewifiinterface.h"
#include "ns3/energy-source-container.h"
//#include "ns3/gtk-config-store.h"
//#include "ns3/li-ion-energy-source-helper.h"
using namespace ns3;
std::ofstream tp ("throughput.txt");
float tptime=0.5;
/*void callbackltewifiinterface(Ptr<LteWifiInterface> ltewifi)
{
	ltewifi->ReadData();
	////std::cout<<"callback called"<<std::endl;
}*/
/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */
NS_LOG_COMPONENT_DEFINE ("EpcFirstExample");
double recvlen=0;
void PacketSinkRxTrace(std::string context, Ptr<const Packet> packet, const Address &address) {
	//////std::cout << "recvlen " << recvlen << std::endl;
	/*packet->Print(////std::cout);
	////std::cout << std::endl;
	////std::cout << packet->GetSize() << std::endl;*/
	recvlen+=packet->GetSize();
}
void printthrpt() {
	std::cout << "Throughput of network " << (double)recvlen/0.5/1024/1024 << std::endl;
        tp<<tptime<<" THp "<<(double)recvlen/0.5/1024/1024 <<" \n ";tptime+=0.5;
	recvlen=0;
	Simulator::Schedule(Seconds(0.5),&printthrpt);
}

void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy << "J");
}
void
TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Total energy consumed by radio = " << totalEnergy << "J");
}
void PrintGnuplottableUeListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++it)
    {
      Ptr<Node> node = *it;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          Ptr<LteUeNetDevice> uedev = node->GetDevice (j)->GetObject <LteUeNetDevice> ();
          if (uedev)
            {
              Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
              outFile << "set label \"" << uedev->GetImsi ()
                      << "\" at "<< pos.x << "," << pos.y << " left font \"Helvetica,4\" textcolor rgb \"grey\" front point pt 1 ps 0.3 lc rgb \"grey\" offset 0,0"
                      << std::endl;
            }
        }
    }
}
void 
PrintGnuplottableEnbListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++it)
    {
      Ptr<Node> node = *it;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          Ptr<LteEnbNetDevice> enbdev = node->GetDevice (j)->GetObject <LteEnbNetDevice> ();
          if (enbdev)
            {
              Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
              outFile << "set label \"" << enbdev->GetCellId ()
                      << "\" at "<< pos.x << "," << pos.y
                      << " left font \"Helvetica,4\" textcolor rgb \"white\" front  point pt 2 ps 0.3 lc rgb \"white\" offset 0,0"
                      << std::endl;
            }
        }
    }
}
/*
void
PrintGnuplottableBuildingListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str (), std::ios_base::out | std::ios_base::trunc);
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  uint32_t index = 0;
  for (BuildingList::Iterator it = BuildingList::Begin (); it != BuildingList::End (); ++it)
    {
      ++index;
      Box box = (*it)->GetBoundaries ();
      outFile << "set object " << index
              << " rect from " << box.xMin  << "," << box.yMin
              << " to "   << box.xMax  << "," << box.yMax
              << " front fs empty "
              << std::endl;
    }
}
*/

int
main (int argc, char *argv[])
{       
	int seedval=1;

	//LogComponentEnable("MinstrelWifiManager",LOG_LEVEL_ALL);

	int lte=1;
	uint16_t numberOfNodes = 7;

	uint16_t numberOfClients=70;//50;//Users
	
	double simTime = 10;
	double distance = 100;
	long xvalue=1;
	long yvalue=2;
	//int flag=0;
	/*if(flag==0) {
		//printthrpt();
		flag=1;
	}*/
	double interPacketInterval =5461;//1024==8 mbps
	//2048 = 4 Mbps
	//4096 = 2 Mbps
	//8192==1 mbps
	ns3::PacketMetadata::Enable ();
	//start Command line arguments
	CommandLine cmd;
	cmd.AddValue("numberOfNodes", "Number of eNodeBs + UE pairs", numberOfNodes);
	cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
	cmd.AddValue("distance", "Distance between eNBs [m]", distance);
	cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
	cmd.AddValue("xvalue", "X co-ordinate postion", xvalue);
	cmd.AddValue("yvalue", "Y co-ordinate postion", yvalue);
	cmd.AddValue("lte", "1- only lte, 2- only wifi, 3- Lte+wifi, 4- LTE UL and Wi-Fi DL", lte);
	cmd.AddValue("seedval", "1 or more", seedval);
	cmd.Parse(argc, argv);
        //stop Command line arguments

	ns3::RngSeedManager::SetSeed(seedval) ;
	
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
	Ptr<PointToPointEpcHelper>  epcHelper = CreateObject<PointToPointEpcHelper> ();
	lteHelper->SetEnbAntennaModelType ("ns3::IsotropicAntennaModel");
	lteHelper->SetEpcHelper (epcHelper);
        
	ConfigStore inputConfig;
	inputConfig.ConfigureDefaults();
	Ipv4Interface::onlylte=lte;
	EpcEnbApplication::onlylte=lte;

	Ptr<Node> pgw = epcHelper->GetPgwNode ();
	
        // Create a single RemoteHost
	NodeContainer remoteHostContainer;
	remoteHostContainer.Create (1);
	Ptr<Node> remoteHost = remoteHostContainer.Get (0);
        InternetStackHelper internet;
	internet.Install (remoteHostContainer);
	
     
	// Create the Internet
	PointToPointHelper p2ph;
	p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
	p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
	//p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
	NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
	
	Ipv4AddressHelper ipv4h;
	ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
	Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
	// interface 0 is localhost, 1 is the p2p device
	Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);
	
	
	
	////std::cout<<"Remote host Address  : "<<remoteHostAddr<<std::endl;

	Ipv4StaticRoutingHelper ipv4RoutingHelper;
	Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
	remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);
	
	
        NodeContainer integrated_box[numberOfNodes];
        //NodeContainer integrated_boxs;
	NodeContainer integrated_ue;
	//NodeContainer ueNodes;
	NodeContainer enbNodes;
	//NodeContainer wifiStaNodes;
	NodeContainer wifiAp;
	
	enbNodes.Create(numberOfNodes);
	wifiAp.Create(numberOfNodes);

	//ADD wi-fi and LTE into integrated box
        for (uint16_t i = 0; i < numberOfNodes; i++)
	{
		
	integrated_box[i].Add(enbNodes.Get(i)); /// careful when more than 1 enb is added
	integrated_box[i].Add(wifiAp.Get(i));
     //   integrated_boxs.Add(integrated_box[i]);
         }
	//ADD Wi-Fi and LTE into UE (Lets call wifi with lte devices as integrated UE)
        integrated_ue.Create(numberOfClients);
	
	


	// Install Mobility Model for Integrated Boxs
/*	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	
	positionAlloc->Add (Vector(0, 0, 0));
        positionAlloc->Add (Vector(0, distance, 0));
        positionAlloc->Add (Vector(0.866*distance, 0.5*distance, 0));
        positionAlloc->Add (Vector(0, -1*distance, 0));
       	positionAlloc->Add (Vector(0.866*distance, -0.5*distance, 0));
        
        positionAlloc->Add (Vector(-0.866*distance, -0.5*distance, 0));
        positionAlloc->Add (Vector(-0.866*distance, 0.5*distance, 0));
        
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.SetPositionAllocator(positionAlloc);

	mobility.Install(integrated_boxs);*/

	MobilityHelper mobility0;
	Ptr<ListPositionAllocator> positionAlloc0 = CreateObject<ListPositionAllocator> ();
	positionAlloc0->Add (Vector(0, 0, 0));
	mobility0.SetPositionAllocator(positionAlloc0);
	mobility0.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility0.Install(integrated_box[0]);

	MobilityHelper mobility1;
	Ptr<ListPositionAllocator> positionAlloc1 = CreateObject<ListPositionAllocator> ();
	positionAlloc1->Add (Vector(0, distance, 0));
	mobility1.SetPositionAllocator(positionAlloc1);
	mobility1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility1.Install(integrated_box[1]);

	MobilityHelper mobility2;
	Ptr<ListPositionAllocator> positionAlloc2 = CreateObject<ListPositionAllocator> ();
	positionAlloc2->Add (Vector(0.866*distance, 0.5*distance, 0));
	mobility2.SetPositionAllocator(positionAlloc2);
	mobility2.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility2.Install(integrated_box[2]);

	MobilityHelper mobility3;
	Ptr<ListPositionAllocator> positionAlloc3 = CreateObject<ListPositionAllocator> ();
	positionAlloc3->Add (Vector(0.866*distance, -0.5*distance, 0));
	mobility3.SetPositionAllocator(positionAlloc3);
	mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility3.Install(integrated_box[3]);

	MobilityHelper mobility4;
	Ptr<ListPositionAllocator> positionAlloc4 = CreateObject<ListPositionAllocator> ();
	positionAlloc4->Add (Vector(0, -1*distance, 0));
	mobility4.SetPositionAllocator(positionAlloc4);
	mobility4.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility4.Install(integrated_box[4]);

	MobilityHelper mobility5;
	Ptr<ListPositionAllocator> positionAlloc5 = CreateObject<ListPositionAllocator> ();
	positionAlloc5->Add (Vector(-0.866*distance, -0.5*distance, 0));
	mobility5.SetPositionAllocator(positionAlloc5);
	mobility5.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility5.Install(integrated_box[5]);

	MobilityHelper mobility6;
	Ptr<ListPositionAllocator> positionAlloc6 = CreateObject<ListPositionAllocator> ();
	positionAlloc6->Add (Vector(-0.866*distance, 0.5*distance, 0));
	mobility6.SetPositionAllocator(positionAlloc6);
	mobility6.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility6.Install(integrated_box[6]);


	
	// Install Mobility Model for Integrated UEs
	MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("0.0"),"Y",StringValue("0.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 0; i < 10; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("0.0"),"Y",StringValue("100"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 10; i < 20; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("86.6"),"Y",StringValue("50.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 20; i < 30; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("86.6"),"Y",StringValue("-50.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 30; i < 40; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("0.0"),"Y",StringValue("-100.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 40; i < 50; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("-86.6"),"Y",StringValue("-50.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 50; i < 60; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }


        mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator","X",StringValue("-86.6"),"Y",StringValue("50.0"),"Rho",StringValue("ns3::UniformRandomVariable[Min=5|Max=25]"),"Theta",StringValue("ns3::UniformRandomVariable[Min=0|Max=360]"));
        for (uint16_t i = 60; i < 70; i++)
	   {
	   mobility.Install(integrated_ue.Get(i));
	 }





	
	
 	Config::SetDefault("ns3::LteEnbPhy::TxPower",DoubleValue(23));
	//  Ptr<LteWifiInterface> ltewifiinterface = CreateObject<LteWifiInterface>();
	Config::SetDefault("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue(160));
	// WifiNetDevice::ltewifiinterfaceobject=ltewifiinterface;
	//WiFi Channel is been set
        Config::SetDefault("ns3::YansWifiPhy::TxPowerEnd",DoubleValue(12));
        Config::SetDefault("ns3::YansWifiPhy::TxPowerStart",DoubleValue(12));

        //WifiHelper wifi=WifiHelper::Default ();
	WifiHelper wifi1=WifiHelper::Default ();
	WifiHelper wifi2=WifiHelper::Default ();
	WifiHelper wifi3=WifiHelper::Default ();
	WifiHelper wifi4=WifiHelper::Default ();
	WifiHelper wifi5=WifiHelper::Default ();
	WifiHelper wifi6=WifiHelper::Default ();
	WifiHelper wifi7=WifiHelper::Default ();
	

	//wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi1.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi2.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi3.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi4.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi5.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi6.SetStandard (WIFI_PHY_STANDARD_80211a);
	wifi7.SetStandard (WIFI_PHY_STANDARD_80211a);
	


	//YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel3 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel4 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel5 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel6 = YansWifiChannelHelper::Default ();
	YansWifiChannelHelper channel7 = YansWifiChannelHelper::Default ();
	


	//YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
         Config::SetDefault("ns3::YansWifiPhy::TxPowerEnd",DoubleValue(12));
        Config::SetDefault("ns3::YansWifiPhy::TxPowerStart",DoubleValue(12));

	YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default ();
	YansWifiPhyHelper phy4 = YansWifiPhyHelper::Default ();
	YansWifiPhyHelper phy6 = YansWifiPhyHelper::Default ();

         Config::SetDefault("ns3::YansWifiPhy::TxPowerEnd",DoubleValue(20));
        Config::SetDefault("ns3::YansWifiPhy::TxPowerStart",DoubleValue(20));

	YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default ();
	YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default ();
	YansWifiPhyHelper phy5 = YansWifiPhyHelper::Default ();
	YansWifiPhyHelper phy7 = YansWifiPhyHelper::Default ();
	

	//phy.SetChannel (channel.Create ());
	phy1.SetChannel (channel1.Create ());
	phy2.SetChannel (channel2.Create ());
	phy3.SetChannel (channel3.Create ());
	phy4.SetChannel (channel4.Create ());
	phy5.SetChannel (channel5.Create ());
	phy6.SetChannel (channel6.Create ());
	phy7.SetChannel (channel7.Create ());
	

	//wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi1.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi2.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi3.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi4.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi5.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi6.SetRemoteStationManager ("ns3::AarfWifiManager");
	wifi7.SetRemoteStationManager ("ns3::AarfWifiManager");
	
        
        
	//Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("990000"));
	// disable rts cts all the time.
	// Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("99000"));

	
	NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

	// HtWifiMacHelper mac = HtWifiMacHelper::Default ();

        // Install LTE Devices to the nodes and wifi devices to node
        lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");
	NetDeviceContainer enbLteDevs ;
        Config::SetDefault("ns3::LteEnbPhy::TxPower",DoubleValue(23));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(1)));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(3)));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(5)));
        Config::SetDefault("ns3::LteEnbPhy::TxPower",DoubleValue(15));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(0)));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(2)));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(4)));
        enbLteDevs.Add(lteHelper->InstallEnbDevice (enbNodes.Get(6)));
	
	//lteHelper->SetAttribute("UlBandwidth",UintegerValue(50));
	//enbLteDevs.Get(0)->SetAttribute("UlBandwidth",UintegerValue (50));
	//	ns3::UintegerValue ul;
	//	enbLteDevs.Get(0)->GetAttribute("UlBandwidth",ul);
	//	////std::cout<<"Up link bandwidth = "<<ul.Get() <<std::endl;
	//	ns3::UintegerValue ul_lteup_wifidown=50;
	//	enbLteDevs.Get(0)->SetAttribute("UlBandwidth",ul_lteup_wifidown);
	//	ns3::UintegerValue ul_check;
	//	enbLteDevs.Get(0)->GetAttribute("UlBandwidth",ul_check);
	//	////std::cout<<"Up link bandwidth after setting = "<<ul_check.Get() <<std::endl;
	NetDeviceContainer ueLteDevs;
	for(uint16_t i=0;i<numberOfClients;i++) {
		ueLteDevs.Add(lteHelper->InstallUeDevice (integrated_ue.Get(i)));
	}

	/*int i=0;
	if(i==3|| i==4) {
		phy.Set("ShortGuardEnable",BooleanValue(true));
	}
	//double datarate = 58.5;
	StringValue DataRate;
	if (i==0)
	{
		DataRate = StringValue("OfdmRate54Mbps");
		//datarate = 54;
	}
	else if (i==1)
	{
		DataRate = StringValue("OfdmRate58_5MbpsBW20MHz");
		//datarate = 58.5;
	}
	else if (i == 2)
	{
		DataRate = StringValue("OfdmRate65MbpsBW20MHz");
	//	datarate = 65;
	}
	else if (i == 3)
	{
		DataRate = StringValue("OfdmRate57_8MbpsBW20MHz");
	//	datarate = 57.8;
	}
	else if (i == 4)
	{
		DataRate = StringValue("OfdmRate72_2MbpsBW20MHz");
	//	datarate = 72.2;
	}

	 wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", DataRate,"ControlMode", DataRate);*/
	//std::cout<<"datarate= "<<datarate<<std::endl;
	
	/*bool enableCtsRts=false;
    UintegerValue ctsThr = (enableCtsRts ? UintegerValue (1024) : UintegerValue (2347));
   Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);*/

	NetDeviceContainer wifiDevs[numberOfClients];
	
	Ssid ssid0 = Ssid ("Integrated_Box0");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid0),
			"ActiveProbing", BooleanValue (true));

	for(uint16_t i=0;i<10;i++) 
	{
		wifiDevs[i]=wifi1.Install(phy1,mac,integrated_ue.Get(i));
	}

	Ssid ssid1 = Ssid ("Integrated_Box1");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid1),
			"ActiveProbing", BooleanValue (true));
	for(uint16_t i=10;i<20;i++) 
	{
		wifiDevs[i]=wifi2.Install(phy2,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}

	Ssid ssid2 = Ssid ("Integrated_Box2");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid2),
			"ActiveProbing", BooleanValue (true));
	for(uint16_t i=20;i<30;i++)
	{
		wifiDevs[i]=wifi3.Install(phy3,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}

	Ssid ssid3 = Ssid ("Integrated_Box3");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid3),
			"ActiveProbing", BooleanValue (true));
	for(uint16_t i=30;i<40;i++)
	{
		wifiDevs[i]=wifi4.Install(phy4,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}

	Ssid ssid4 = Ssid ("Integrated_Box4");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid4),
			"ActiveProbing", BooleanValue (true));
	for(uint16_t i=40;i<50;i++)
	{
		wifiDevs[i]=wifi5.Install(phy5,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}

	Ssid ssid5 = Ssid ("Integrated_Box5");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid5),
			"ActiveProbing", BooleanValue (true));
	
	for(uint16_t i=50;i<60;i++)
	{
		wifiDevs[i]=wifi6.Install(phy6,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}

	Ssid ssid6 = Ssid ("Integrated_Box6");
	mac.SetType ("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid6),
			"ActiveProbing", BooleanValue (true));
	
	for(uint16_t i=60;i<70;i++)
	{
		wifiDevs[i]=wifi7.Install(phy7,mac,integrated_ue.Get(i));
		//std::cout <<"dest address " <<wifiDevs[i].Get(0)->g << std::endl;
	}



	
	//ueLteDevs.Get(0)->setLteWifiInterface(ltewifiinterface);
	
	

       mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid0));
	NetDeviceContainer wifiApDevs0 =wifi1.Install(phy1,mac,wifiAp.Get(0));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid1));
	NetDeviceContainer wifiApDevs1 =wifi2.Install(phy2,mac,wifiAp.Get(1));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid2));
	NetDeviceContainer wifiApDevs2 =wifi3.Install(phy3,mac,wifiAp.Get(2));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid3));
	NetDeviceContainer wifiApDevs3 =wifi4.Install(phy4,mac,wifiAp.Get(3));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid4));
	NetDeviceContainer wifiApDevs4 =wifi5.Install(phy5,mac,wifiAp.Get(4));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid5));
	NetDeviceContainer wifiApDevs5 =wifi6.Install(phy6,mac,wifiAp.Get(5));

	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid6));
	NetDeviceContainer wifiApDevs6 =wifi7.Install(phy7,mac,wifiAp.Get(6));


	
	//  mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid));
	// Install the IP stack on the UEs///////////////////////////////////
	InternetStackHelper stack;
	//for (uint16_t i = 0; i < numberOfNodes; i++)
	//  {
	stack.Install (integrated_ue);
	
	// }
	stack.Install(wifiAp);
	/*////std::cout<<"Number of net devices"<<integrated_ue.Get(0)->GetNDevices()<<std::endl;
  ////std::cout<<"Number of net deviceswifi"<<integrated_ue.Get(1)->GetNDevices()<<std::endl;
for (uint32_t i=0;i<integrated_ue.Get(0)->GetNDevices();i++){
	////std::cout<<"Type ID:"<<integrated_ue.Get(0)->GetDevice(i)->GetTypeId().GetName()<<std::endl;
}*/

	//stack.Install (integrated_ue);
	//////std::cout<<"Node size"<<integrated_ue.GetN()<<std::endl;
	//internet.Install (integrated_ue.Get(1));
        RouterLayer::epc_enb_wifi_netdevice[0]=wifiApDevs0.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[1]=wifiApDevs1.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[2]=wifiApDevs2.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[3]=wifiApDevs3.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[4]=wifiApDevs4.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[5]=wifiApDevs5.Get(0);
	RouterLayer::epc_enb_wifi_netdevice[6]=wifiApDevs6.Get(0);
	
     
        
        //std::cout<<"Node size"<<wifiApDevs.GetN()<<std::endl;
	//internet.Install (integrated_box);
	Ipv4InterfaceContainer ueIpIface;
	ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));
	


        Mac48Address wifiapmacaddress0 = "22:33:44:55:66:09";
	wifiApDevs0.Get(0)->SetAddress(wifiapmacaddress0);
	ns3::RouterLayer::wifi_Ap_macaddr[0]=wifiapmacaddress0;

	Mac48Address wifiapmacaddress1 = "22:33:44:55:66:19";
	wifiApDevs1.Get(0)->SetAddress(wifiapmacaddress1);
	ns3::RouterLayer::wifi_Ap_macaddr[1]=wifiapmacaddress1;

	Mac48Address wifiapmacaddress2 = "22:33:44:55:66:29";
	wifiApDevs2.Get(0)->SetAddress(wifiapmacaddress2);
	ns3::RouterLayer::wifi_Ap_macaddr[2]=wifiapmacaddress2;

	Mac48Address wifiapmacaddress3 = "22:33:44:55:66:39";
	wifiApDevs3.Get(0)->SetAddress(wifiapmacaddress3);
	ns3::RouterLayer::wifi_Ap_macaddr[3]=wifiapmacaddress3;

	Mac48Address wifiapmacaddress4 = "22:33:44:55:66:49";
	wifiApDevs4.Get(0)->SetAddress(wifiapmacaddress4);
	ns3::RouterLayer::wifi_Ap_macaddr[4]=wifiapmacaddress4;

	Mac48Address wifiapmacaddress5 = "22:33:44:55:66:59";
	wifiApDevs5.Get(0)->SetAddress(wifiapmacaddress5);
	ns3::RouterLayer::wifi_Ap_macaddr[5]=wifiapmacaddress5;

	Mac48Address wifiapmacaddress6 = "22:33:44:55:66:69";
	wifiApDevs6.Get(0)->SetAddress(wifiapmacaddress6);
	ns3::RouterLayer::wifi_Ap_macaddr[6]=wifiapmacaddress6;




	Mac48Address wifimacaddress;
	ns3::RouterLayer router;

  	// Assign  address to clients
	for(uint16_t i=0;i<numberOfClients;i++) {
		std::string mac="22:33:44:55:67:70";
		int a = i+10;
		std::stringstream ss;
		ss << a;
		std::string mac_last_prefix = ss.str();
		mac.append(mac_last_prefix);
		wifimacaddress=mac.c_str();
               
		//std::cout << "client mac address"<<wifimacaddress << std::endl;
		wifiDevs[i].Get(0)->SetAddress(wifimacaddress);
              //  std::cout << "client mac address end" << std::endl;
		uint16_t enb_id=0;

		if (i>=0 && i<10) 
		{
			enb_id=0;
		}
		if (i>=10 && i<20)
		{
			enb_id=1;
		}
		if (i>=20 && i<30)
		{
			enb_id=2;
		}
		if (i>=30 && i<40)
		{
			enb_id=3;
		}
		if (i>=40 && i<50)
		{
			enb_id=4;
		}
		if (i>=50 && i<60)
		{
			enb_id=5;
		}
		if (i>=60 && i<70)
		{
			enb_id=6;
		}
	
	
		router.mapfunc(wifimacaddress,ueIpIface.GetAddress(i),enb_id);
		//router.mapfunc(wifimacaddress,ueIpIface.GetAddress(i));
	}

        Ipv4AddressHelper address;
	for (uint16_t i = 0; i < numberOfClients; i++)
	{
		Ipv4Address adr=ueIpIface.GetAddress(i,0);
		Ipv4InterfaceContainer ueIpIface1;
		ueIpIface1=address.assignaddress(wifiDevs[i],adr);
                ns3::LteHelper::ntoipmap[integrated_ue.Get(i)]=adr;
	//	std::cout<<" UE Interface address"<<ueIpIface.GetAddress(i,0)<<std::endl;
	//	std::cout<<" Wifi Interface address"<<ueIpIface1.GetAddress(0,0)<<std::endl;
	}

	// Assign IP address to UEs, and install applications
	for (uint32_t u = 0; u < numberOfClients; ++u)
	{
		Ptr<Node> ueNode = integrated_ue.Get (u);
		// Set the default gateway for the UE
		Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
		ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
	}


        uint16_t ue_num = 0;
        for(ue_num = 0;ue_num<10;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(0));
	}
	for(ue_num = 10;ue_num<20;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(1));
	}
	for(ue_num = 20;ue_num<30;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(2));
	}
	for(ue_num = 30;ue_num<40;ue_num++)  
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(3));
	}
	for(ue_num = 40;ue_num<50;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(4));
	}
	for(ue_num = 50;ue_num<60;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(5));
	}
	for(ue_num = 60;ue_num<70;ue_num++) 
	{
		lteHelper->Attach (ueLteDevs.Get(ue_num), enbLteDevs.Get(6));
	}






      //-----------------------------------------------------------------------------------------------------------------------------------------------
               /** Energy Model **/
  /***************************************************************************/
  /* energy source */
   LiIonEnergySourceHelper LiIonSourceHelper;
  // configure energy source
  LiIonSourceHelper.Set ("LiIonEnergySourceInitialEnergyJ", DoubleValue (31752.0));
  // install source
  EnergySourceContainer sources=LiIonSourceHelper.Install (integrated_ue) ;
 
/*for(uint16_t i=0;i<numberOfClients;i++) {
		
            sources[i] = LiIonSourceHelper.Install (integrated_ue.Get(i));
	}
 */  


/*  device energy model */



  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0174));
  // install device model
 //DeviceEnergyModelContainer deviceModels =radioEnergyHelper.Install (wifiDevsEnergy, sources);

      NetDeviceContainer wifiDevsEnergy;
     for(uint16_t i=0;i<numberOfClients;i++) {
 		
             wifiDevsEnergy.Add(wifiDevs[i]);
	

      }
DeviceEnergyModelContainer deviceModels =radioEnergyHelper.Install (wifiDevsEnergy, sources);
/*for(uint16_t i=0;i<numberOfClients;i++) {
		

	deviceModels[i] = radioEnergyHelper.Install (wifiDevs[i], sources.Get(i));

      }*/
/*  **************************************************************************/






  /** connect trace sources **/
  /***************************************************************************/
  // all sources are connected to node 1
  // energy source
      //    Ptr<LiIonEnergySource> LiIonSourcePtr = DynamicCast<LiIonEnergySource> (sources.Get(0));
      //    LiIonSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeCallback (&RemainingEnergy));
  // device energy model
//    Ptr<DeviceEnergyModel> LiIonRadioModelPtr =  LiIonSourcePtr->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  //NS_ASSERT (LiIonRadioModelPtr != NULL);
  //LiIonRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
  /***************************************************************************/










 

/*	for(uint16_t i=0;i<numberOfClients;i++) {
		std::string mac="22:33:44:55:66:7";
		int a = i;
		std::stringstream ss;
		ss << a;
		std::string mac_last_prefix = ss.str();
		mac.append(mac_last_prefix);
		wifimacaddress=mac.c_str();
		////std::cout << "client mac address"<<wifimacaddress << std::endl;
		wifiDevs[i].Get(0)->SetAddress(wifimacaddress);
		router.mapfunc(wifimacaddress,ueIpIface.GetAddress(i));
                if(i<10)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[3];
                else if(i<20)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[5];
                else if(i<30)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[0];
                else if(i<40)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[4];
                else if(i<50)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[2];
                else if(i<60)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[6];
                else if(i<70)
                ns3::RouterLayer::ueenbmap[wifimacaddress]=ns3::RouterLayer::wifi_Ap_macaddr[1];
	}



	//wifiDevs.Get(0)->setLteWifiInterface(ltewifiinterface);
	// ltewifiinterface->SetDevices(wifiDevs,ueLteDevs);


	//Ipv4Address ipaddr="7.0.0.2";
	//wifiDevs.Get(0)->SetAddress(ipaddr);

	Ipv4AddressHelper address;
	// address.SetBase ("8.0.0.0", "255.0.0.0");
	//Ipv4Address adr1="7.0.0.5";


	//Just commented
	for (uint16_t i = 0; i < numberOfClients; i++)
	{
		Ipv4Address adr=ueIpIface.GetAddress(i,0);
		Ipv4InterfaceContainer ueIpIface1;
		ueIpIface1=address.assignaddress(wifiDevs[i],adr);
                ns3::LteHelper::ntoipmap[integrated_ue.Get(i)]=adr; 
		////std::cout<<" UE Interface address"<<ueIpIface.GetAddress(i,0)<<std::endl;
		////std::cout<<" Wifi Interface address"<<ueIpIface1.GetAddress(0,0)<<std::endl;
	}*/
	/*for (uint16_t i = 0; i < numberOfClients; i++)
		{
			Ipv4Address adr=ueIpIface.GetAddress(i,0);
			std::cout << adr << std::endl;
		}
	std::cout << "extra ues\n";
	for (uint16_t i = 0; i < extraues; i++)
			{
				Ipv4Address adr=extraueIpIface.GetAddress(i,0);
				std::cout << adr << std::endl;
			}*/
	/*for (NodeContainer::Iterator j = integrated_ue.Begin ();
			j != integrated_ue.End (); ++j)
	{
		Ptr<Node> object = *j;
		Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
		NS_ASSERT (position != 0);
		Vector pos = position->GetPosition ();
		////std::cout << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
	}*/



	//ueIpIface2=address.assignaddress(wifiApDevs,adr1);
	//  address.Assign(wifiApDevs);

	//////std::cout<<"Wfi AP address"<<ueIpIface2.GetAddress(0,0)<<std::endl;
	//wifiDevs.Get(0)->SetAddress()


	// Assign IP address to UEs, and install applications
	//  for (uint32_t u = 0; u < integrated_ue.GetN (); ++u)
	/*for (uint32_t u = 0; u < numberOfClients; ++u)
	{
		Ptr<Node> ueNode = integrated_ue.Get (u);

		// Set the default gateway for the UE
		Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
		ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

	}
	
*/
	// Attach all UE to eNodeB
       // lteHelper->AttachToClosestEnb (ueLteDevs, enbLteDevs);
	//Only 1 ue
	for (uint16_t i = 0; i < numberOfClients; i++)
	{
		



		if(i<16)
		{

			enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;  // define Qci type
			GbrQosInformation qos;
			qos.gbrDl = 87200; // Downlink GBR
			qos.gbrUl = 87200; // Uplink GBR
			qos.mbrDl = 87200; // Downlink MBR
			qos.mbrUl = 87200; // Uplink MBR
			EpsBearer bearer (q, qos);
			lteHelper->ActivateDedicatedEpsBearer (ueLteDevs.Get(i), bearer, EpcTft::Default ());
			EpcEnbApplication epcenbapp;
			Callback<void,Ipv4Address,EpsBearer,double> cb;
			cb = MakeCallback(&EpcEnbApplication::addbearer, &epcenbapp);
			cb(ueIpIface.GetAddress (i),bearer,(double)qos.gbrDl/1000000);


		} else if(i<36)
		{

			enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;  // define Qci type
			GbrQosInformation qos;
			qos.gbrDl = 1200000; // Downlink GBR
			qos.gbrUl = 1200000; // Uplink GBR
			qos.mbrDl = 1200000; // Downlink MBR
			qos.mbrUl = 1200000; // Uplink MBR
			EpsBearer bearer (q, qos);
			lteHelper->ActivateDedicatedEpsBearer (ueLteDevs.Get(i), bearer, EpcTft::Default ());
			EpcEnbApplication epcenbapp;
			Callback<void,Ipv4Address,EpsBearer,double> cb;
			cb = MakeCallback(&EpcEnbApplication::addbearer, &epcenbapp);
			cb(ueIpIface.GetAddress (i),bearer,(double)qos.gbrDl/1000000);
		} else if(i<46)
		{
			enum EpsBearer::Qci q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;  // define Qci type
			GbrQosInformation qos;
			qos.gbrDl = 0; // Downlink GBR
			qos.gbrUl = 0; // Uplink GBR
			qos.mbrDl = 0; // Downlink MBR
			qos.mbrUl = 0; // Uplink MBR
			EpsBearer bearer (q, qos);
			lteHelper->ActivateDedicatedEpsBearer (ueLteDevs.Get(i), bearer, EpcTft::Default ());
			EpcEnbApplication epcenbapp;
			Callback<void,Ipv4Address,EpsBearer,double> cb;
			cb = MakeCallback(&EpcEnbApplication::addbearer, &epcenbapp);
			cb(ueIpIface.GetAddress (i),bearer,(double)qos.gbrDl/1000000);
		} else if(i<50)
		{
			enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;  // define Qci type
			GbrQosInformation qos;
			qos.gbrDl = 1200000; // Downlink GBR
			qos.gbrUl = 1200000; // Uplink GBR
			qos.mbrDl = 1200000; // Downlink MBR
			qos.mbrUl = 1200000; // Uplink MBR
			EpsBearer bearer (q, qos);
			lteHelper->ActivateDedicatedEpsBearer (ueLteDevs.Get(i), bearer, EpcTft::Default ());
			EpcEnbApplication epcenbapp;
			Callback<void,Ipv4Address,EpsBearer,double> cb;
			cb = MakeCallback(&EpcEnbApplication::addbearer, &epcenbapp);
			cb(ueIpIface.GetAddress (i),bearer,(double)qos.gbrDl/1000000);
		} else
		{

			enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;  // define Qci type
			GbrQosInformation qos;
			qos.gbrDl = 1200000; // Downlink GBR
			qos.gbrUl = 1200000; // Uplink GBR
			qos.mbrDl = 1200000; // Downlink MBR
			qos.mbrUl = 1200000; // Uplink MBR
			EpsBearer bearer (q, qos);
			lteHelper->ActivateDedicatedEpsBearer (ueLteDevs.Get(i), bearer, EpcTft::Default ());
			EpcEnbApplication epcenbapp;
			Callback<void,Ipv4Address,EpsBearer,double> cb;
			cb = MakeCallback(&EpcEnbApplication::addbearer, &epcenbapp);
			cb(ueIpIface.GetAddress (i),bearer,(double)qos.gbrDl/1000000);

		}
		//        WifiNetDevice::LteDevs=ueLteDevs.Get(i);
		//Ptr<LteUeNetDevice> ueLteDevice = ueLteDevs.Get(i)->GetObject<LteUeNetDevice> ();
		//Ptr<ns3::Packet> pkt;
		//ueLteDevice->GetNas()->DoRecvData(pkt);
		// WifiNetDevice::ueNas = ueLteDevice->GetNas ();
		// side effect: the default EPS bearer will be activated

	}

	

	//wifiDevs.Get(0)->SetAddress(ueLteDevs.Get(0)->GetAddress());//setting same mac address of LTE
	//Ipv4InterfaceContainer IpIfacewifiDevs=ipv4h.Assign(wifiDevs);
	//////std::cout<<"wifi mac address"<<wifiDevs.Get(0)->GetAddress()<<std::endl;
	//////std::cout<<"wifi Ap address"<<wifiApDevs.Get(0)->GetAddress()<<std::endl;
	//////std::cout<<"wifi mac address"<<wifiDevs.Get(1)->GetAddress()<<std::endl;
	//////std::cout<<"wifi Ap address"<<wifiApDevs.Get(1)->GetAddress()<<std::endl;

	// ////std::cout<<"epc_enb_wifi_netdevice"<<EpcEnbApplication::epc_enb_wifi_netdevice->GetAddress()<<std::endl;


	// Install and start applications on UEs and remote host
	//TCP Application============================
	/*uint16_t port=2001;
	uint16_t port1=1001;
	for (uint32_t u = 0; u < numberOfClients; ++u)
	{
		BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (remoteHostAddr, port));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("MaxBytes", UintegerValue (0));
		source.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (1.0));
		sinkApps.Stop (Seconds (10.0));
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;


		BulkSendHelper source1 ("ns3::TcpSocketFactory",InetSocketAddress (remoteHostAddr, port));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source1.SetAttribute ("MaxBytes", UintegerValue (0));
		source1.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps1 = source1.Install (integrated_ue.Get(u));
		sourceApps1.Start (Seconds (1.0));
		sourceApps1.Stop (Seconds (10.0));
		PacketSinkHelper sink1 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps1 = sink1.Install (remoteHost);
		sinkApps1.Start (Seconds (1.0));
		sinkApps1.Stop (Seconds (10.0));
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;


		BulkSendHelper source2 ("ns3::TcpSocketFactory",InetSocketAddress (ueIpIface.GetAddress (u), port1));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source2.SetAttribute ("MaxBytes", UintegerValue (0));
		source2.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps2 = source2.Install (remoteHost);
		sourceApps2.Start (Seconds (1.0));
		sourceApps2.Stop (Seconds (10.0));
		PacketSinkHelper sink2 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps2 = sink2.Install (integrated_ue.Get(u));
		sinkApps2.Start (Seconds (1.0));
		sinkApps2.Stop (Seconds (10.0));
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		BulkSendHelper source3 ("ns3::TcpSocketFactory",InetSocketAddress (ueIpIface.GetAddress (u), port1));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source3.SetAttribute ("MaxBytes", UintegerValue (0));
		source3.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps3 = source3.Install (remoteHost);
		sourceApps3.Start (Seconds (1.0));
		sourceApps3.Stop (Seconds (10.0));
		PacketSinkHelper sink3 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps3 = sink3.Install (integrated_ue.Get(u));
		sinkApps3.Start (Seconds (1.0));
		sinkApps3.Stop (Seconds (10.0));
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

	}*/
/*
Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();

// PrintGnuplottableBuildingListToFile ("buildings.txt");
      PrintGnuplottableEnbListToFile ("enbs.txt");
      PrintGnuplottableUeListToFile ("ues.txt");
  remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/3"));
  remHelper->SetAttribute ("OutputFile", StringValue ("rem.out"));
  remHelper->SetAttribute ("XMin", DoubleValue (-500.0));
  remHelper->SetAttribute ("XMax", DoubleValue (+500.0));
  remHelper->SetAttribute ("YMin", DoubleValue (-500.0));
  remHelper->SetAttribute ("YMax", DoubleValue (+500.0));
  remHelper->SetAttribute ("Z", DoubleValue (1.5));
  //remHelper->SetAttribute ("UseDataChannel", BooleanValue (true));
  //remHelper->SetAttribute ("RbId", IntegerValue (10));
  remHelper->Install ();
*/
	//UDP Application======================================


 uint16_t port=2000;
	uint16_t port1=1001;
	double count=0;
	for (uint32_t u = 0; u <8; u++)
	{
		UdpClientHelper source (remoteHostAddr, port);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (0.0));
		sinkApps.Stop (Seconds (10.0));

		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
        
	//count=0;
	
   	for (uint32_t u = 0; u < numberOfClients; u++)
	{//------------------------test
		//if(u<32) {
		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		//-----------------------------try
		
	}for (uint32_t u = 0; u <numberOfClients ; u++)
	{
		//------------------------test
		//if(u<32) {
		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		//-----------------------------try
	}
     for (uint32_t u = 0; u <numberOfClients ; u++)
	{//------------------------test
		//if(u<32) {
		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		//-----------------------------try
		
	}
        for (uint32_t u = 8; u <16; ++u)
	{
		//------------------------test

		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));

		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		//-----------------------------try
	}
	double temp=count+0.1;
	for (uint32_t u = 16; u < 26; u++)
	{
		UdpClientHelper source (remoteHostAddr, port);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (0.0));
		sinkApps.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
	count=temp;
	for (uint32_t u = 26; u < 36; ++u)
	{
		//------------------------test
		//if(u<32) {
		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		//-----------------------------try
	}
	//temp=count;
	for(uint32_t u=36;u<41;u++) {
		BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (remoteHostAddr, port));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("MaxBytes", UintegerValue (0));
		source.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (5.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (5.0));
		sinkApps.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
         for(uint32_t u=36;u<41;u++) {

                     UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));

		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;


           }
	for (uint32_t u = 41; u < 46; u++)
	{
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		BulkSendHelper source2 ("ns3::TcpSocketFactory",InetSocketAddress (ueIpIface.GetAddress (u), port1));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source2.SetAttribute ("MaxBytes", UintegerValue (0));
		source2.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps2 = source2.Install (remoteHost);
		sourceApps2.Start (Seconds (5.0));
		sourceApps2.Stop (Seconds (10.0));
		PacketSinkHelper sink2 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps2 = sink2.Install (integrated_ue.Get(u));

		sinkApps2.Start (Seconds (0.0));
		sinkApps2.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		//std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
	}
	//count=temp;
    for (uint32_t u = 41; u < 46; u++)
	{
                UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));

		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
         }
	for (uint32_t u = 46; u < 49; ++u)
	{
		//------------------------test

		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		//-----------------------------try
	}






	/*uint16_t port=2000;
	uint16_t port1=1001;
	double count=0;
	for (uint32_t u = 0; u < 8; u++)
	{
		UdpClientHelper source (remoteHostAddr, port);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (0.0));
		sinkApps.Stop (Seconds (10.0));

		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
	//count=0;
	for (uint32_t u = 8; u <16; ++u)
	{
		//------------------------test

		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(94160)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));

		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		//-----------------------------try
	}
	double temp=count+0.1;
	for (uint32_t u = 16; u < 26; u++)
	{
		UdpClientHelper source (remoteHostAddr, port);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (0.0));
		sinkApps.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
	count=temp;
	for (uint32_t u = 26; u < 36; ++u)
	{
		//------------------------test
		//if(u<32) {
		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		//-----------------------------try
	}
	//temp=count;
	for(uint32_t u=36;u<41;u++) {
		BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (remoteHostAddr, port));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("MaxBytes", UintegerValue (0));
		source.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));
		sourceApps.Start (Seconds (5.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (5.0));
		sinkApps.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}
	for (uint32_t u = 41; u < 46; u++)
	{
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
		BulkSendHelper source2 ("ns3::TcpSocketFactory",InetSocketAddress (ueIpIface.GetAddress (u), port1));
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source2.SetAttribute ("MaxBytes", UintegerValue (0));
		source2.SetAttribute("SendSize",UintegerValue (1024));
		ApplicationContainer sourceApps2 = source2.Install (remoteHost);
		sourceApps2.Start (Seconds (5.0));
		sourceApps2.Stop (Seconds (10.0));
		PacketSinkHelper sink2 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps2 = sink2.Install (integrated_ue.Get(u));

		sinkApps2.Start (Seconds (0.0));
		sinkApps2.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		//std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;
	}
	//count=temp;
	for (uint32_t u = 46; u < 49; ++u)
	{
		//------------------------test

		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		//-----------------------------try
	}*/



	/*for (uint32_t u = 49; u < 54; u++)
	{
		UdpClientHelper source (remoteHostAddr, port);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install (integrated_ue.Get(u));


		sourceApps.Start (Seconds (1.0));
		sourceApps.Stop (Seconds (10.0));
		PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
		ApplicationContainer sinkApps = sink.Install (remoteHost);
		sinkApps.Start (Seconds (0.0));
		sinkApps.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Uplink UE = "<<u<<" Source Port "<<port<<std::endl;
		port++;
	}


	for (uint32_t u = 54; u < numberOfClients; u++)
	{
		//------------------------test

		UdpClientHelper source11 (ueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(6826)));
		source11.SetAttribute ("MaxPackets", UintegerValue(100000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (remoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (integrated_ue.Get(u));
		sinkApps11.Start (Seconds (0.0));
		sinkApps11.Stop (Seconds (10.0));
		if(u%2==0)
			count=count+0.3;
		////std::cout<<"Downlink UE = "<<u<<" Source Port "<<port1<<std::endl;
		port1++;

		//-----------------------------try
	}



*/
	//=======================================================================================

	    //  uint16_t port=2000;
	//uint16_t port1=1001;
 /*	for (uint32_t u = 0; u < extraues; ++u)
	{
		UdpClientHelper source (remoteHostAddr, port);
  	 	    // Set the amount of data to send in bytes.  Zero is unlimited.
  	  source.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
  	  source.SetAttribute ("MaxPackets", UintegerValue(1000000));
  	  source.SetAttribute ("PacketSize", UintegerValue(1024));
  	  ApplicationContainer sourceApps = source.Install (extrauenodes.Get(u));
  	  sourceApps.Start (Seconds (1.0));
  	   sourceApps.Stop (Seconds (10.0));
  	   PacketSinkHelper sink ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
  	   ApplicationContainer sinkApps = sink.Install (remoteHost);
  	   sinkApps.Start (Seconds (1.0));
  	   sinkApps.Stop (Seconds (10.0));
  	   port++;
		
           }

 









  	   //------------------------test
  	   UdpClientHelper source01 (remoteHostAddr, port);
  	   	 	    // Set the amount of data to send in bytes.  Zero is unlimited.
  	   	  source01.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
  	   	  source01.SetAttribute ("MaxPackets", UintegerValue(1000000));
  	   	  source01.SetAttribute ("PacketSize", UintegerValue(1024));
  	   	  ApplicationContainer sourceApps01 = source01.Install (integrated_ue.Get(u));
  	   	  sourceApps01.Start (Seconds (1.0));
  	   	   sourceApps01.Stop (Seconds (3.0));
  	   	   PacketSinkHelper sink01 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
  	   	   ApplicationContainer sinkApps01 = sink01.Install (remoteHost);
  	   	   sinkApps01.Start (Seconds (1.0));
  	   	   sinkApps01.Stop (Seconds (3.0));
  	   	   port++;
  	   //-----------------------------try


  	   UdpClientHelper source1 (ueIpIface.GetAddress (u), port1);
  	   	 	    // Set the amount of data to send in bytes.  Zero is unlimited.
  	    // Set the amount of data to send in bytes.  Zero is unlimited.
  	     source1.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
  	  	  source1.SetAttribute ("MaxPackets", UintegerValue(10000000));
  	  	  source1.SetAttribute ("PacketSize", UintegerValue(1024));
  	   	  ApplicationContainer sourceApps1 = source1.Install (remoteHost);
  	   	  sourceApps1.Start (Seconds (1.0));
  	   	   sourceApps1.Stop (Seconds (3.0));
  	   	   PacketSinkHelper sink1 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
  	   	   ApplicationContainer sinkApps1 = sink1.Install (integrated_ue.Get(u));
  	   	   sinkApps1.Start (Seconds (1.0));
  	   	   sinkApps1.Stop (Seconds (3.0));
  	   	   port1++;

		//------------------------test
				UdpClientHelper source11 (extraueIpIface.GetAddress (u), port1);
		// Set the amount of data to send in bytes.  Zero is unlimited.
		source11.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
		source11.SetAttribute ("MaxPackets", UintegerValue(1000000));
		source11.SetAttribute ("PacketSize", UintegerValue(1024));
		ApplicationContainer sourceApps11 = source11.Install (extraremoteHost);
		sourceApps11.Start (Seconds (1.0));
		sourceApps11.Stop (Seconds (10.0));
		PacketSinkHelper sink11 ("ns3::UdpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
		ApplicationContainer sinkApps11 = sink11.Install (extrauenodes.Get(u));
		sinkApps11.Start (Seconds (1.0));
		sinkApps11.Stop (Seconds (10.0));
		port1++;
		//std::cout << extraremoteHostAddr << std::endl;
		//-----------------------------try
		 */


	//}




	/* uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  //uint16_t otherPort = 3000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;
  //for (uint32_t u = 0; u < integrated_ue.GetN (); ++u)

  for (uint32_t u = 0; u < numberOfClients; ++u)
    {
	  ////std::cout<<"UE number"<<u<<std::endl;
     // ++ulPort;
      //++otherPort;
	  ++dlPort;
	  ////std::cout << "port " << dlPort << std::endl;
      PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
    //  PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
//      PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
    // serverApps.Add (dlPacketSinkHelper.Install (integrated_ue.Get(u)));
     // serverApps.Add (ulPacketSinkHelper.Install (integrated_ue.Get(u)));

      serverApps.Add (dlPacketSinkHelper.Install (integrated_ue.Get(u)));
     // serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
//      serverApps.Add (packetSinkHelper.Install (integrated_ue.Get(u)));

      UdpClientHelper dlClient (ueIpIface.GetAddress (u), dlPort);

      dlClient.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue(1000000));
      dlClient.SetAttribute ("PacketSize", UintegerValue(1024));

//      UdpClientHelper ulClient (remoteHostAddr, ulPort);
//      ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
//      ulClient.SetAttribute ("MaxPackets", UintegerValue(1000000));
//
     // UdpClientHelper client (ueIpIface.GetAddress (u), dlPort);
     // client.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
     // client.SetAttribute ("MaxPackets", UintegerValue(1000000));
     // client.SetAttribute ("PacketSize", UintegerValue(1024));

      clientApps.Add (dlClient.Install (remoteHost));
     // clientApps.Add (client.Install (remoteHost));
//      if (u+1 < integrated_ue.GetN ())
//        {
//          clientApps.Add (client.Install (integrated_ue.Get(u+1)));
//        }
//      else
//        {
//          clientApps.Add (client.Install (integrated_ue.Get(0)));
//        }
      ////std::cout<<"UE number"<<u<<std::endl;
            ++ulPort;
            //++otherPort;
      	 // ++dlPort;
      	 ////std::cout << "port " << dlPort << std::endl;
           // PacketSinkHelper dlPacketSinkHelper1 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
           PacketSinkHelper ulPacketSinkHelper1 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      //      PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
          // serverApps.Add (dlPacketSinkHelper.Install (integrated_ue.Get(u)));
           // serverApps.Add (ulPacketSinkHelper.Install (integrated_ue.Get(u)));

         //   serverApps.Add (dlPacketSinkHelper1.Install (integrated_ue));
            serverApps.Add (ulPacketSinkHelper1.Install (remoteHost));
      //      serverApps.Add (packetSinkHelper.Install (integrated_ue.Get(u)));

            UdpClientHelper dlClient1 (remoteHostAddr, dlPort);

            dlClient1.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
            dlClient1.SetAttribute ("MaxPackets", UintegerValue(1000000));
            dlClient1.SetAttribute ("PacketSize", UintegerValue(1024));

      //      UdpClientHelper ulClient (remoteHostAddr, ulPort);
      //      ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      //      ulClient.SetAttribute ("MaxPackets", UintegerValue(1000000));
      //

            clientApps.Add (dlClient1.Install (integrated_ue.Get(u)));

    }
  serverApps.Start (Seconds (1.0));
  clientApps.Start (Seconds (1.0));
  serverApps.Stop(Seconds(3.0));
  clientApps.Stop(Seconds(3.0));*/

         
 

	lteHelper->EnableTraces ();

	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor;

	monitor= flowmon.Install (integrated_ue);

	flowmon.Install (remoteHost);
	/*for(uint16_t i=0;i<numberOfClients;i++) {
		phy.EnablePcap ("Wifi", wifiDevs[i]);
	}*/
        
	for(uint16_t i=0;i<numberOfClients;i++) {
		

		if (i>=0 && i<10) 
		{
			phy1.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=10 && i<20)
		{
			phy2.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=20 && i<30)
		{
			phy3.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=30 && i<40)
		{
			phy4.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=40 && i<50)
		{
			phy5.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=50 && i<60)
		{
			phy6.EnablePcap ("Wifi", wifiDevs[i]);
		}
		if (i>=60 && i<70)
		{
			phy7.EnablePcap ("Wifi", wifiDevs[i]);
		}
	
	
		
	}

	phy1.EnablePcap ("WifiAp", wifiAp);
	phy2.EnablePcap ("WifiAp", wifiAp);
	phy3.EnablePcap ("WifiAp", wifiAp);
	phy4.EnablePcap ("WifiAp", wifiAp);
	phy5.EnablePcap ("WifiAp", wifiAp);
	phy6.EnablePcap ("WifiAp", wifiAp);
	phy7.EnablePcap ("WifiAp", wifiAp);
	// epcHelper->EnablePcap("Lte",ueLteDevs);
	Config::Connect("/NodeList/*/DeviceList/*/LteUePhy/ReportCurrentCellRsrpSinr", MakeCallback(&EpcEnbApplication::updateltesinrper));
	Simulator::Stop(Seconds(simTime));

	// Uncomment to enable PCAP tracing
	//p2ph.EnablePcapAll("lena-epc-first");
	//Simulator::Schedule(MilliSeconds(100),&callbackltewifiinterface,ltewifiinterface);
	//AsciiTraceHelper ascii;
	//epcHelper->EnableAsciiAll (ascii.CreateFileStream ("trace.tr"));


  	Simulator::Run();

	/*GtkConfigStore config;
  config.ConfigureAttributes();*/

	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	//uint32_t lostpkt=0;
	double Thrpt=0;double received_bytes=0, received_pkts=0, transmitted_packets=0;ns3::Time total_time;
	double Delay=0,PLoss=0;
	double uplinkthrpt=0,downlinkthrpt=0,uplinkdelay=0,downlinkdelay=0,uplinkpacketloss=0,downlinkpacketloss=0;
	double uplinktxpkts=0, downlinktxpkts=0, uplinkrecvpkts=0, downlinkrecvpkts=0;
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
	{

		if (1)
		{

			Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);

			std::cout << "Flow " << i->first << " (" << t.sourceAddress << "(" << t.sourcePort <<")" << " -> " << t.destinationAddress <<"("<<t.destinationPort<<")"<< ")\n";
			std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
			std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
			received_bytes+=i->second.rxBytes;
			std::cout << "  Transmitted Packets: " << i->second.txPackets << std::endl;
			std::cout << "  Received Packets: " << i->second.rxPackets << std::endl;
			std::cout << "  First Tx time:   " << i->second.timeFirstTxPacket << "\n";
			std::cout << "  Last Rx time:   " << i->second.timeLastRxPacket << "\n";
			std::cout << "  Delay = " << (i->second.delaySum.GetSeconds()/i->second.rxPackets*1000)<< "msec \n";
			total_time+=i->second.timeLastRxPacket-i->second.timeFirstTxPacket;
			if((i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=0 && (i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=std::numeric_limits<double>::quiet_NaN() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::infinity() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::quiet_NaN()) {
				//std::cout << (double)i->second.rxBytes*8 << " " << i->second.timeLastRxPacket - i->second.timeFirstTxPacket << std::endl;
				std::cout << "  Throughput: " << ( ((double)i->second.rxBytes*8) / (i->second.timeLastRxPacket - i->second.timeFirstTxPacket).GetSeconds()/1024/1024 ) << "Mbps" << std::endl;
				Thrpt +=( ((double)i->second.rxBytes*8) / (i->second.timeLastRxPacket - i->second.timeFirstTxPacket).GetSeconds()/1024/1024 );
			} else {
				////std::cout << "  Throughput: " << 0 << std::endl;
			}
			if(i->second.delaySum.GetSeconds()!=std::numeric_limits<double>::infinity() && i->second.delaySum.GetSeconds()!=std::numeric_limits<double>::quiet_NaN())
				Delay += i->second.delaySum.GetSeconds();
			else
				Delay+=0;
			received_pkts+=i->second.rxPackets;
			//  ////std::cout << "  local Delay = " << i->second.delaySum.GetSeconds()/i->second.rxPackets << "\n";
			PLoss+=i->second.txPackets-i->second.rxPackets ;
			transmitted_packets+=i->second.txPackets;
			if(t.destinationPort>=2000) {
				////std::cout << "reachced uplnk\n";
				if((i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=0 && (i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=std::numeric_limits<double>::quiet_NaN() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::infinity() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::quiet_NaN())
					uplinkthrpt+=( ((double)i->second.rxBytes*8) / (i->second.timeLastRxPacket - i->second.timeFirstTxPacket).GetSeconds()/1024/1024 );
				uplinkdelay+=i->second.delaySum.GetSeconds();
				uplinkpacketloss+=i->second.txPackets-i->second.rxPackets ;
				uplinktxpkts+=i->second.txPackets;
				uplinkrecvpkts+=i->second.rxPackets;
			} else {
				////std::cout << "reachced downlnk\n";
				if((i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=0 && (i->second.timeLastRxPacket - i->second.timeFirstTxPacket)!=std::numeric_limits<double>::quiet_NaN() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::infinity() && (double)i->second.rxBytes*8!=std::numeric_limits<double>::quiet_NaN())
					downlinkthrpt+=( ((double)i->second.rxBytes*8) / (i->second.timeLastRxPacket - i->second.timeFirstTxPacket).GetSeconds()/1024/1024 );
				downlinkdelay+=i->second.delaySum.GetSeconds();
				downlinkpacketloss+=i->second.txPackets-i->second.rxPackets ;
				downlinktxpkts+=i->second.txPackets;
				downlinkrecvpkts+=i->second.rxPackets;
			}
			//lostpkt+=i->second.lostPackets;
			//    std::iterator<int,float> s=i->second.packetsDropped.iterator;

		}
	}
	std::cout << "uplink thrpt" << uplinkthrpt << std::endl;
	std::cout << "downlink thrpt" << downlinkthrpt << std::endl;
	//std::cout << "uplink delay" << uplinkdelay/uplinkrecvpkts << std::endl;
	//std::cout << "downlink delay" <<  downlinkdelay/downlinkrecvpkts << std::endl;
	//std::cout << "uplink packetloss" << uplinkpacketloss/uplinktxpkts << std::endl;
	//std::cout << "downlink packetloss" << downlinkpacketloss/downlinktxpkts << std::endl;
	//std::cout << "  Packet loss = " << PLoss<< "\n";
	//std::cout << "Percentage of Lost packets = "<<((PLoss/transmitted_packets)*100)<<std::endl;
	//std::cout << "Total  Delay = " << (Delay/received_pkts)<< "\n";
	//std::cout << " Total Rx Bytes: " << received_bytes;
	// ////std::cout << " Total Lost Pkt : " << lostpkt;
	//////std::cout << " Expected Throughput : " << (received_bytes*8)/total_time;
	std::cout << " Total Throughput: " << Thrpt <<std::endl;
        //std::cout << "maximize throughput:"<<mxpt <<std::endl;
        //std::cout << "move fairly:"<<fair <<std::endl;



      for (uint16_t i = 0; i < numberOfClients; i++)
	{
	   Ipv4Address adr=ns3::LteHelper::ntoipmap[integrated_ue.Get(i)];
             double   battery=ns3::LteHelper::iptoenergymap[adr];
            std::cout <<  integrated_ue.Get(i)<<"   "<<adr<<" "<<battery <<std::endl;

     }
// ns3::EpcEnbApplication::flows.close();
// ns3::EpcEnbApplication::total.close();
//	Simulator::Destroy();
	//return 0;


	return 0;




}

