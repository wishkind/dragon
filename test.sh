#!bin/bash
if [ -f "DDD" ]; then
	echo "deng"
	exec 5>dev-debug.log
	BASH_XTRACEFD="5"
	set -x
fi
	
