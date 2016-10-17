#!/bin/sh
#
# (c) Copyright IBM Corp. 2016
#
# Change Activity:
#

# Get linux version
name=`cat /etc/*release|grep ^NAME`
lnx_name=`echo ${name#*\"}|awk '{print $1}'`
version=`cat /etc/*release|grep ^VERSION`
lnx_version=`echo ${version#*\"}|awk '{print $1}'|cut -d "." -f 1`

echo $lnx_name
echo $lnx_version

# Replace iucvserver files
if [[ "$lnx_name" = "Red" && $lnx_version -lt 7 ]] || [[ "$lnx_name" = "Suse" && $lnx_version -lt 12 ]]; then
    service iucvserd stop
    mv /etc/init.d/iucvserd /etc/init.d/iucvserd.old
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv
    mv /etc/init.d/iucvserd.new /etc/init.d/iucvserd
    service iucvserd restart
    # roll back
    ps_iucv=`ps -ef|grep -c iucv`
    if [[ $ps_iucv -lt 2 ]]; then
        mv /etc/init.d/iucvserd.old /etc/init.d/iucvserd
    	mv /usr/bin/iucvserv.old /usr/bin/iucvserv
	service iucvserd restart
    fi
fi

if [[ "$lnx_name" = "Red" && $lnx_version -ge 7 ]] || [[ "$lnx_name" = "Ubuntu" ]]; then
    systemctl  stop iucvserd
    mv /lib/systemd/system/iucvserd.service /lib/systemd/system/iucvserd.service.old
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv
    mv /lib/systemd/system/iucvserd.service.new /lib/systemd/system/iucvserd.service
    systemctl daemon-reload
    systemctl restart iucvserd
    # roll back
    ps_iucv=`ps -ef|grep -c iucv`
    if [[ $ps_iucv -lt 2 ]]; then
        mv /lib/systemd/system/iucvserd.service.old /lib/systemd/system/iucvserd.service
    	mv /usr/bin/iucvserv.old /usr/bin/iucvserv
	systemctl restart iucvserd
    fi
fi

if [[ "$lnx_name" = "Suse" && $lnx_version -ge 12 ]]; then
    systemctl  stop iucvserd
    mv /usr/lib/systemd/system/iucvserd.service /usr/lib/systemd/system/iucvserd.service.old
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv
    mv /usr/lib/systemd/system/iucvserd.service.new /usr/lib/systemd/system/iucvserd.service
    systemctl daemon-reload
    systemctl restart iucvserd
    # roll back
    ps_iucv=`ps -ef|grep -c iucv`
    if [[ $ps_iucv -lt 2 ]]; then
        mv /usr/lib/systemd/system/iucvserd.service.old /usr/lib/systemd/system/iucvserd.service
    	mv /usr/bin/iucvserv.old /usr/bin/iucvserv
	systemctl restart iucvserd
    fi
fi
