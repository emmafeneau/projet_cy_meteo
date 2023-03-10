# Constants
readonly P3GNUPLOTCMD='plot "output/p3.dat" using 1:2'
readonly T3GNUPLOTCMD='plot "output/t3.dat" using 1:2'
readonly NOERROR=0
readonly OPTIONERROR=1
readonly INFILEERROR=2
readonly OUTFILEERROR=3
readonly OTHERERROR=4
readonly GCC=/usr/bin/gcc
readonly CEXEC='./meteo'
readonly LIBDIR='lib/'
readonly TMPDIR='tmp/'
readonly INPUTDIR='input/'
readonly OUTPUTDIR='output/'
readonly P3OUTPUTDIR=$OUTPUTDIR'p3/'
readonly T3OUTPUTDIR=$OUTPUTDIR't3/'
readonly RESULTDIR='results/'
readonly OBJDIR='obj/'
readonly directories=("$TMPDIR" "$OUTPUTDIR" "$RESULTDIR" "$OBJDIR" "$P3OUTPUTDIR" "$T3OUTPUTDIR")
readonly TRUE=1
readonly FALSE=0
readonly SECONDSPERDAY=86400
readonly PERIODFILTER='|tr -d ":"'
readonly EMPTYFIELDFILTER='" ;\|;;\|;\r\n\|;\r\|;\n\|;$\|; "'
readonly QUOTEDQUOTES='"'
readonly CSVSEPARATOR=';'
readonly TIMESEPARATOR='T'
readonly FILETERMCSV='.csv'
readonly FILETERMDAT='.dat'
readonly FILETERMSH='.sh'
readonly FILETERMPNG='.png'
readonly MAXLOCATIONS=1
readonly MAXDATES=255
readonly MAXSORTINGOPTIONS=1
readonly tmpFile=$TMPDIR'tmp.csv'
readonly zoneFilteredFile=$TMPDIR'tmp_zone_filtered.csv'
readonly timeFilteredFile=$TMPDIR'tmp_time_filtered.csv'
readonly CITIESFILE='cities.csv'
readonly shortOptions=("-t" "-p" "-w" "-h" "-m" "-d" "-F" "-G" "-S" "-A" "-O" "-Q" "-f")
readonly longOptions=("--avl" "--abr" "--tab")
readonly mandatoryOptions=("-t" "-p" "-w" "-h" "-m")
readonly locationOptions=("-F" "-G" "-S" "-A" "-O" "-Q")
readonly dateOption="-d"
readonly inputOption="-f"
readonly helpOption="--help"
readonly shortArgsNumber=("-t:1" "-p:1" "-w:0" "-h:0" "-m:0" "-d:2" "-F:0" "-G:0" "-S:0" "-A:0" "-O:0" "-Q:0" "-f:1")
readonly errorPattern='Error :' 
readonly defaultError='wrong option, options combination or argument '
# ZG;long max;long min;lat max;lat min
readonly limits=("-F;9.5616;-4.7886;51.0833;41.3334" "-G;-51.6404;-54.4569;5.7370;2.1484" \
                 "-S;-56.1282;-56.4034;47.1422;46.7520" "-A;-60.8149;-62.9769;18.1247;14.4001" \
                 "-O;55.8287;45.0438;-12.6380;-21.3809" "-Q;180.0;0.0;-65.874;-90.0")