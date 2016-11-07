#!/bin/bash

dir="/home/ssrluser/Downloads/ns3_old/ns-allinone-3.19_1/ns-3.19/maxresults_using802.11a/log_lteulwifidl_4_ue_udp/"

mkdir $dir

	logfile11=`echo -n "${dir}/log_1ue_4_position1.log"`
        logfile21=`echo -n "${dir}/log_1ue_24_position1.log"`
        logfile31=`echo -n "${dir}/log_1ue_48_position1.log"`
        logfile41=`echo -n "${dir}/log_1ue_96_position1.log"` 
	logfile12=`echo -n "${dir}/log_1ue_4_position2.log"`
        logfile22=`echo -n "${dir}/log_1ue_24_position2.log"`
        logfile32=`echo -n "${dir}/log_1ue_48_position2.log"`
        logfile42=`echo -n "${dir}/log_1ue_96_position2.log"` 	
	logfile13=`echo -n "${dir}/log_1ue_4_position3.log"`
        logfile23=`echo -n "${dir}/log_1ue_24_position3.log"`
        logfile33=`echo -n "${dir}/log_1ue_48_position3.log"`
        logfile43=`echo -n "${dir}/log_1ue_96_position3.log"` 
	logfile14=`echo -n "${dir}/log_1ue_4_position4.log"`
        logfile24=`echo -n "${dir}/log_1ue_24_position4.log"`
        logfile34=`echo -n "${dir}/log_1ue_48_position4.log"`
        logfile44=`echo -n "${dir}/log_1ue_96_position4.log"` 
	logfile15=`echo -n "${dir}/log_1ue_4_position5.log"`
        logfile25=`echo -n "${dir}/log_1ue_24_position5.log"`
        logfile35=`echo -n "${dir}/log_1ue_48_position5.log"`
        logfile45=`echo -n "${dir}/log_1ue_96_position5.log"` 
        #logfile5=`echo -n "${dir}/log_1ue_lte(ctrl+uldata)wifi(dldata)_position.log"`
	#logfile6=`echo -n "${dir}/log_4ue_lteulwifidl_position.log"`
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=1" > $logfile11 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=4096  --lte=4 --seedval=1" > $logfile21 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=2048  --lte=4 --seedval=1" > $logfile31 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=1" > $logfile41 2> /dev/null; echo "finished 4")&
	 (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=2" > $logfile12 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=4096  --lte=4 --seedval=2" > $logfile22 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=2048  --lte=4 --seedval=2" > $logfile32 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=2" > $logfile42 2> /dev/null; echo "finished 4")&
	 (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=3" > $logfile13 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=4096  --lte=4 --seedval=3" > $logfile23 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=2048  --lte=4 --seedval=3" > $logfile33 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=3" > $logfile43 2> /dev/null; echo "finished 4")&
	 (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=4" > $logfile14 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=4096  --lte=4 --seedval=4" > $logfile24 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=2048  --lte=4 --seedval=4" > $logfile34 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=4" > $logfile44 2> /dev/null; echo "finished 4")&
	 (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=8192 --lte=4 --seedval=5" > $logfile15 2> /dev/null; echo "finished 1")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=4096  --lte=4 --seedval=5" > $logfile25 2> /dev/null; echo "finished 2")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=2048  --lte=4 --seedval=5" > $logfile35 2> /dev/null; echo "finished 3")&
        (./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=4 --seedval=5" > $logfile45 2> /dev/null; echo "finished 4")&
        #(./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=1365  --lte=5" > $logfile5 2> /dev/null; echo "finished 5")& 
        #(./waf --run "scratch/lena-simple-epc_802_11_n  --interPacketInterval=133  --lte=6" > $logfile6 2> /dev/null; echo "finished 6")& 
        echo "waiting"
        wait
        
	echo "finished waiting"
    
echo "Experiment is completed !!!!!!!!!!!!!"
cd $dir
grep -r "Total Throughput" . > ../throughput_log_1_ue_udp.txt
grep -r "Total  Delay" . > ../Delay_log__1_ue_udp.txt
grep -r "Percentage of Lost packets" . > ../Ploss_log__1_ue_udp.txt
grep -r "uplink thrpt" . > ../uplinkthroughput_log_1_ue_udp.txt
grep -r "uplink delay" . > ../uplinkDelay_log__1_ue_udp.txt
grep -r "uplink packetloss" . > ../uplinkPloss_log__1_ue_udp.txt
grep -r "downlink thrpt" . > ../downlinkthroughput_log_1_ue_udp.txt
grep -r "downlink delay" . > ../downlinkDelay_log__1_ue_udp.txt
grep -r "downlink packetloss" . > ../downlinkPloss_log__1_ue_udp.txt

chmod -R 777 .
