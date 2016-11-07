#!/bin/bash

dir="/home/ssrluser/Downloads/ns3_old/ns-allinone-3.19_1/ns-3.19/maxresults_using802.11a/log_ltewifi_flowsplit_1_ue_tcp/"

mkdir $dir

	logfile11=`echo -n "${dir}/log_1ue_position1.log"`
        logfile21=`echo -n "${dir}/log_1ue_position2.log"`
        logfile31=`echo -n "${dir}/log_1ue_position3.log"`
        logfile41=`echo -n "${dir}/log_1ue_position4.log"` 
  	logfile12=`echo -n "${dir}/log_1ue_position5.log"` 
        #logfile5=`echo -n "${dir}/log_1ue_lte(ctrl+uldata)wifi(dldata)_position.log"`
	#logfile6=`echo -n "${dir}/log_4ue_lteulwifidl_position.log"`

        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=1" > $logfile11 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=2" > $logfile21 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=682  --lte=4 --seedval=3" > $logfile31 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=341  --lte=4 --seedval=4" > $logfile41 2> /dev/null; echo "finished 4")&
	 (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=5" > $logfile12 2> /dev/null; echo "finished 1")&
        #(./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=5" > $logfile5 2> /dev/null; echo "finished 5")& 
        #(./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=133  --lte=6" > $logfile6 2> /dev/null; echo "finished 6")& 
        echo "waiting"
        wait
        
	echo "finished waiting"
    
echo "Experiment is completed !!!!!!!!!!!!!"
cd $dir
grep -r "Total Throughput" . > ../throughput_log_1_ue_udp.txt
grep -r "uplink thrpt" . > ../uplinkthroughput_log_1_ue_udp.txt
grep -r "downlink thrpt" . > ../downlinkthroughput_log_1_ue_udp.txt

chmod -R 777 .
