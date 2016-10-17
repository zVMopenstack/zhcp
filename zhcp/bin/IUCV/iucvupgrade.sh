#!/bin/sh
#
# IBM(c) 2013-2016 EPL license http://www.eclipse.org/legal/epl-v10.html
#
# Change Activity:
#

# Get linux version
name=`cat /etc/*release|grep ^NAME`
lnx_name=`echo ${name#*\"}|awk '{print $1}'`
version=`cat /etc/*release|grep ^VERSION`
lnx_version=`echo ${version#*\"}|awk '{print $1}'|cut -d "." -f 1`

echo $lnx_name $lnx_version

rollback_rhel6_sles11(){
    if [ -f /etc/init.d/iucvserd.old ]; then
        mv /etc/init.d/iucvserd.old /etc/init.d/iucvserd 2>/dev/nul
    fi
    if [ -f /usr/bin/iucvserv.old ]; then
        mv /usr/bin/iucvserv.old /usr/bin/iucvserv 2>/dev/nul
    fi
    service iucvserd restart
}

rollback_rhel7_ubuntu(){
    if [ -f /lib/systemd/system/iucvserd.service.old ]; then
        mv /lib/systemd/system/iucvserd.service.old /lib/systemd/system/iucvserd.service 2>/dev/nul
    fi
    if [ -f /usr/bin/iucvserv.old ]; then
        mv /usr/bin/iucvserv.old /usr/bin/iucvserv 2>/dev/nul
    fi
    systemctl restart iucvserd
}

rollback_sles12(){
    if [ -f /usr/lib/systemd/system/iucvserd.service.old ]; then
        mv /usr/lib/systemd/system/iucvserd.service.old /lib/systemd/system/iucvserd.service 2>/dev/nul
    fi
    if [ -f /usr/bin/iucvserv.old ]; then
        mv /usr/bin/iucvserv.old /usr/bin/iucvserv 2>/dev/nul
    fi
    systemctl restart iucvserd
}

# Replace iucvserver files
# Rhel6 and Sles11
if [[ "$lnx_name" = "Red" && $lnx_version -lt 7 ]] || [[ "$lnx_name" = "Suse" && $lnx_version -lt 12 ]]; then
    service iucvserd stop
    mv /etc/init.d/iucvserd /etc/init.d/iucvserd.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel6_sles11
        exit 1
    fi
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel6_sles11
        exit 1
    fi
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel6_sles11
        exit 1
    fi
    mv /etc/init.d/iucvserd.new /etc/init.d/iucvserd 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel6_sles11
        exit 1
    fi
    service iucvserd restart
    # roll back
    ps_iucv=`ps -ef|grep -c iucvserv`
    if [[ $ps_iucv -lt 2 ]]; then
        rollback_rhel6_sles11
        exit 1
    fi

# Above Rhel7 and Ubuntu
elif [[ "$lnx_name" = "Red" && $lnx_version -ge 7 ]] || [[ "$lnx_name" = "Ubuntu" ]]; then
    pkill iucvserv
    mv /lib/systemd/system/iucvserd.service /lib/systemd/system/iucvserd.service.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel7_ubuntu
        exit 1
    fi
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel7_ubuntu
        exit 1
    fi
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel7_ubuntu
        exit 1
    fi
    mv /lib/systemd/system/iucvserd.service.new /lib/systemd/system/iucvserd.service 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_rhel7_ubuntu
        exit 1
    fi
    systemctl daemon-reload
    systemctl restart iucvserd

    # roll back
    ps_iucv=`ps -ef|grep -c iucvserv`
    if [[ $ps_iucv -lt 2 ]]; then
        rollback_rhel7_ubuntu
        exit 1
    fi

# Above Sles12
elif [[ "$lnx_name" = "Suse" && $lnx_version -ge 12 ]]; then
    pkill iucvserv
    mv /usr/lib/systemd/system/iucvserd.service /usr/lib/systemd/system/iucvserd.service.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_sles12
        exit 1
    fi
    mv /usr/bin/iucvserv /usr/bin/iucvserv.old 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_sles12
        exit 1
    fi
    mv /usr/bin/iucvserv.new /usr/bin/iucvserv 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_sles12
        exit 1
    fi
    mv /usr/lib/systemd/system/iucvserd.service.new /usr/lib/systemd/system/iucvserd.service 2>/dev/nul
    if [ $? -ne 0 ]; then
        rollback_sles12
        exit 1
    fi
    systemctl daemon-reload
    systemctl restart iucvserd
    # roll back
    ps_iucv=`ps -ef|grep -c iucvserv`
    if [[ $ps_iucv -lt 2 ]]; then
        rollback_sles12
        exit 1
    fi
fi
