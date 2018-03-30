#!/bin/sh

set -e

if [ $# -lt 2 ]; then
    echo >&2 "Insufficient parameters"
    echo >&2 "Usage:  templates.sh <RT-Druid_dir> <template_id> <output_dir>"
    exit 2
fi
LAUNCHER_JAR=`find "$1/plugins" -name "org.eclipse.equinox.launcher_*.jar" | sort | tail -1`

case "`uname -s`" in
    *CYGWIN*)
#
#           CYGWIN
#
	LAUNCHER_JAR="`cygpath -w "$LAUNCHER_JAR"`"
	;;

    *)
#
#           Linux/Unix
#    
	;;
esac


# check if the user asks for the RT-Druid Version  
for a in "$@"; do
    if [ "$a" = "--version" ]; then
        exec java -jar "$LAUNCHER_JAR" -application com.eu.evidence.rtdruid.oil.standalone.writer --version
        exit 0
    fi
done

exec java -jar "$LAUNCHER_JAR" -data "$3/workspace" -application com.eu.evidence.rtdruid.oil.standalone.templates --templateId "$2" --outputDir "$3"
