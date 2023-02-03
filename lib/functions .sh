# function for the help command + description of all options for the user
function displayHelp {
    local scriptName="$0"

    echo
    echo -e '\nUSAGE :' $scriptName [options] [arguments]
    echo 'OPTIONS :'
    echo -e '\ttype of data : At least one of these options must be chosen for the script to work. It is possible to activate several of them: there is no particular limitation at this level.\n'
    echo -e '\t-t : this option must be accompanied by a mode :\n'
    echo -e '\t\t1 : produces an error bar graph\n'
    echo -e '\t\t2 : produces a simple line diagram\n'
    echo -e '\t\t3 : produces a multiple line diagram\n'
    echo -e '\t-p : This option must be accompanied by a mode :\n'    
    echo -e '\t\t1 : produces an error bar graph\n'
    echo -e '\t\t2 : produces a simple line diagram\n'
    echo -e '\t\t3 : produces a multiple line diagram\n'
    echo -e '\t-w : produces a vector diagram\n'
    echo -e '\t-h : produces an interpolated and colored map diagram\n'
    echo -e '\t-m : produces an interpolated and colored map diagram\n'
    echo -e '\tgeographical option : That only one option at a time can be activated at a time. If none is activated, there will be no geographical limitation on the processed measures.\n'
    echo -e '\t-F : Metropolitan France + Corsica\n'      
    echo -e '\t-G : French Guiana\n' 
    echo -e '\t-S : Saint-Pierre and Miquelon: islands located in Eastern Canada\n' 
    echo -e '\t-A : Antilles\n' 
    echo -e '\t-O : French islands in the Indian Ocean\n' 
    echo -e '\t-Q : Antarctica\n' 
    echo -e '\t-f : is the path to the filtered data file. This option is mandatory\n' 
    echo -e '\t-d : must be formatted as follows : YYYY-MM-DD YYYY-MM-DD. This option is not mandatory : there will be no time limitation on the processed measures. It is possible to put several date ranges.\n'
    echo -e '\tsorting method : There can only be one sorting method at a time. If none is activated, the sorting will be done by AVL\n'
    echo -e '\t--avl : sorting method is Balanced Binary Search Tree\n' 
    echo -e '\t-abr : sorting method is by Binary Search Tree\n' 
    echo -e '\t--tab : sorting method is by a Linked List\n'
    echo -e '\t--help : display this manual unconditionaly\n'  
}

# function that creates temporary directories
function createDirectories {
    local dir
    local retval=255

    for dir in "${directories[@]}"
    do
        if ! [ -d "$dir" ]
        then
            mkdir "$dir"
            retval=$?
            if [ $retval -gt 0 ]
            then
                sendReturnCode 1 "Unable to create directory ${dir}"
            fi
        fi
    done 
}

# delete data drom directories when they no longer used
function emptyDirectories {
    local dir

    for dir in "${directories[@]}"
    do    
        find $dir -type f -delete 
    done    
}

# check if help has been requested from command line
function checkHelp {
    IFS=', ' read -r -a params <<< "$@"   

    if [[ "${params[@]}" == *"$helpOption"* ]]
    then
        displayHelp 
        sendReturnCode 0
    fi
}

# check if a valid extract option has been requested
function checkNoExtractOption {     
    local mandatoryOptionFound=$FALSE
    local option=''
    local optionPart=''
    local i=0

    for extOpt in "${extractOptions[@]}"
    do
        optionPart=${extOpt:0:2}     
        for manOpt in "${mandatoryOptions[@]}"
        do
            if [ $optionPart == $manOpt ] 
            then
                mandatoryOptionFound=$TRUE
                if ! [[ "${types[@]}" == *"$extOpt"* ]]
                then
                    types[$i]=$extOpt
                    i=$((i+1))
                fi
            fi
        done
    done
    if [ $mandatoryOptionFound == $FALSE ]
    then
        sendReturnCode 1 "mandatory option not found"
    fi
}

# checks if all mandatory options are set
function checkMandatoryOption {    
    local mandatoryOptionFound=$FALSE    
    local optionPart=''    

    for extOpt in "${extractOptions[@]}"
    do
        optionPart=${extOpt:0:2}      
        if [ $optionPart == $inputOption ] 
        then
            mandatoryOptionFound=$TRUE
        fi
    done
    if [ $mandatoryOptionFound == $FALSE ]
    then
        sendReturnCode 1 "input option not found"
    fi    
}

# check is option is a valid date option
function isDateOption {
    local option=$1
    local count="${#dates[@]}"

    option=${option:0:2}
    if [ "$option" == "$dateOption" ]
    then
        dates[$count]=$option
        count=$((count+1))
        if [ $count -gt $MAXDATES ]
        then
            sendReturnCode 1 "too many dates"
        fi
        return 0
    fi
}

# checks if there is at most one location option
function isLocationOption {  
    local option=$1
    local count="${#locations[@]}"

# get first two characters
    option=${option:0:2}
    for locOpt in "${locationOptions[@]}"
    do
        if [ "$option" == "$locOpt" ]
        then
            locations[$count]=$option
            count=$((count+1))
            if [ $count -gt $MAXLOCATIONS ]
            then
                sendReturnCode 1 "too many locations"
            fi
            return 0
        fi        
    done 
}
# function that checks if the option is short. For example a short option is p,t,m etc
function isShortOption {  
    local option=$1
    local count=0

 # get first two characters
    option=${option:0:2}
    for shOpt in "${shortOptions[@]}"
    do
        if [ "$option" == "$shOpt" ]
        then
            return $count
        fi        
        count=$((count+1))
    done 
    return 255
}
# function that checks if the option is long. For example a short option is avl,abr,tab etc
function isLongOption { 
    local option=$1
    local count=0  

# get first five characters
    option=${option:0:5}
    for loOpt in "${longOptions[@]}"
    do
        if [ "$option" == "$loOpt" ]
        then
            sortingMethod="$loOpt"
            sortingOptionsNumber=$((sortingOptionsNumber+1))
            if [ $sortingOptionsNumber -gt $MAXSORTINGOPTIONS ]
            then
                sendReturnCode 1 'too many sorting options'
            fi
            return $count
        fi        
        count=$((count+1))
    done   
    return 255
}

# check if an option have the right number of his argument
function checkArgumentsNumber {
    local option=$1
    local argument=$2
    local finalArgsNumber=$3
    local argsNumber=255

    for item in "${shortArgsNumber[@]}"
    do
        if [[ "$item" =~ "$option" ]]
        then
            argsNumber="${item//"$option"':'}"
            if [ $argument -gt $argsNumber ]
            then
                sendReturnCode 1  
            elif ! [[ -z $finalArgsNumber ]] && [[ $finalArgsNumber -lt $argsNumber ]]     
            then  
                sendReturnCode 1                         
            fi
            break
        fi
    done
    return $argsNumber
}

# check if an argument is valid for this option
function checkArgument {   
    local option=$1
    local argument=$2

    firstChar=${argument:0:1}
    if [ "$firstChar" == '-' ]
    then
        sendReturnCode 1
    fi    

    case "$option" in
        -p) checkArgTemperaturePressure $option $argument
            ;;
        -t) checkArgTemperaturePressure $option $argument
            ;;
        -d) checkArgDate $option $argument    
            ;;
        -f) fileExists $argument
            ;;
    esac
}

# function that checks if the used input file exists
function fileExists {
    local inputFile=$1

    if ! [ -f "$inputFile" ]
    then
        sendReturnCode 2 'file does not exist' "$inputFile"
    fi
    dataFile=$inputFile
}

# check if there are several times characters
function checkDuplicates {
    local options
    local i
    local j
    local k

    IFS=', ' read -r -a options <<< "$@"    
    for i in "${options}"
    do
        k=$((k+1))
        for j in "${options}"
        do
            if [[ "$j" == "$j" ]] && [[ $k -ge 0 ]] 
            then
                sendReturnCode 1
            fi
        done
    done
}

# check if more than one locatio has been requested
function checkLocations {
    local counter=$1

    if [ $counter -ge $MAXLOCATIONS ]
    then
        sendReturnCode 1
    fi
}

# check if the argument is valid for option p or t
function checkArgTemperaturePressure {
    local option=$1
    local argument=$2

    case "$argument" in
        1)  ;;
        2)  ;;
        3)  ;;
        *)  sendReturnCode 1 $option $argument
            ;;    
    esac        
}

# function that checks if the date is written in the right way
function checkArgDate {     
    local option=$1
    local argument=$2
    local now
    local argDate   
    local count="${#dates[@]}"   
    local i=$((count-1)) 
    local retval=-1

    if [[ $argument =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]
    then
        now=$(date +%s)
        argDate=$(date -d $argument +%s)
        retval=$?
        if [ $retval -gt 0 ]
        then
            sendReturnCode 1 "Wrong date $argument"
        fi
        if [ $date1 -eq -1 ]
        then
            date1=$argDate
        else
            date2=$argDate
            if [ $i -gt -1 ]
            then
                dates[$i]=$option';'$date1';'$date2
            fi
            date1=-1
            date2=-1
        fi
        return 0
    fi
    sendReturnCode 1 "$argument is not a valid date"
}

# function that checks if the first character is an option
function checkFirstCharacter {
    local param=$1
    local firstChar

    firstChar=${param:0:1}
    if [ $firstChar != '-' ]
    then
        sendReturnCode 1 'wrong option' $parameter
    fi
}
# function that checks for bad user input
function checkOptionsAndArguments {
    local i=0
    local j=0
    local k=0
    local currOption=''
    local param=''
    local paramLength=0
    local optionPart=''
    local argPart=''
    local currArgsNumber=0
    local waitingForArg=$FALSE
    local isLongOption=255
    local isShortOption=255
    local retval=255
    local tmpOption=''
    local maxArgsNumber=-1
    local locationsNumber=0
    local params=()
 
    IFS=', ' read -r -a params <<< "$@"    
    for param in "${params[@]}"
    do
        param="${param//' '}"
        if [ $i -eq 0 ]
        then
            checkFirstCharacter $param
        fi
        i=$((i+1))

        # check if param is a long option
        isLongOption $param
        retval=$?      
        if [ $retval -ne 255 ]
        then
            if [ $waitingForArg == $TRUE ]
            then
                sendReturnCode 1
            fi
            continue
        fi        
                
        # check if param is a short option                
        isShortOption $param
        retval=$?
        tmpOption="${shortOptions[$retval]}"

        # check if param is a location option
        isLocationOption $param        

        # check if param is a date option
        isDateOption $param         

        # option is a short option
        if [ $retval -ne 255 ]
        then
            # option has changed
            if [[ "$currOption" != '' ]] && [[ "$currOption" != "$tmpOption" ]]
            then
                # checking previous option for final arguments number
                checkArgumentsNumber $currOption $currArgsNumber $currArgsNumber
                waitingForArg=$FALSE
            fi

            currOption="$tmpOption"
            extractOptions[$j]=$currOption
            k=$j
            j=$((j+1))
            paramLength=${#param}
            if [ $paramLength -gt 2 ]
            then
                optionPart=${param:0:2} 
                argPart="${param//"$optionPart"}"
                currArgsNumber=1
                checkArgument $currOption $argPart
                extractOptions[$k]="${extractOptions[$k]}"';'$argPart
            else
                currArgsNumber=0                
            fi
            checkArgumentsNumber $currOption $currArgsNumber  
            maxArgsNumber=$?
            if [[ $maxArgsNumber -gt 0 ]] && [[ $currArgsNumber != $maxArgsNumber ]]
            then
                waitingForArg=$TRUE
            else
                waitingForArg=$FALSE
            fi                
        else
            checkArgument $currOption "$param"  
            extractOptions[$k]="${extractOptions[$k]}"';'$param            
            currArgsNumber=$((currArgsNumber+1))
            checkArgumentsNumber $currOption $currArgsNumber       
            if [ $currArgsNumber -eq $maxArgsNumber ]
            then
                waitingForArg=$FALSE
            fi 
        fi

    done       
    checkNoExtractOption
    checkMandatoryOption   
}

# get min and max dates from database
function getMinMaxDates() {   
    local cmd=255

    echo 'currently extracting dates'
    cmd=$(tail -n +2 $dataFile |cut -d"$CSVSEPARATOR" -f2 |sort -u > $tmpFile)
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode 1 'unable to filter dates'
    fi
    dataMinDate=$(head -n 1 $tmpFile)
    dataMaxDate=$(tail -n 1 $tmpFile)    
    rm $tmpFile
}

# get geographical bounds for requested zone
function buildZonePattern() { 
    local i=0
    local comp
    local retval=255
    local cmd=255
    local latitude
    local longitude
    local location="${locations[0]}"

    echo '+ currently building geographical zone filter'
    # Get zone bounds
    for limit in "${limits[@]}"
    do
        IFS=';' read -r -a coordinates <<< "$limit"     
        zone="${coordinates[0]}"
        if [ $zone == $location ]
        then
            longMax="${coordinates[1]}"
            longMin="${coordinates[2]}"
            latMax="${coordinates[3]}"
            latMin="${coordinates[4]}"            
            break
        fi
    done

    # get all stations and their coordinates
    cmd=$(tail -n +2 $dataFile |cut -d';' -f1,10 |sort -u > $tmpFile)
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode 1
    fi

    # check stations vs geographic zone
    while IFS=';' read -r dataStationId dataCoordinates   
    do
        IFS=',' read -r -a gpsCoordinates <<< "${dataCoordinates[@]}" 
        latitude="${gpsCoordinates[0]}"
        longitude="${gpsCoordinates[1]}"
        comp=$(echo "$latitude < $latMin" |bc -l)
        if [ $comp -eq 1 ]
        then
            break
        fi
        comp=$(echo "$latitude > $latMax" |bc -l)
        if [ $comp -eq 1 ]
        then
            break
        fi    
        comp=$(echo "$longitude < $longMin" |bc -l)
        if [ $comp -eq 1 ]
        then
            break
        fi    
        comp=$(echo "$longitude > $longMax" |bc -l)
        if [ $comp -eq 1 ]
        then
            break
        fi      
        if [ $i -eq 0 ]
        then
            stationsString='^"'$dataStationId
            i=1
        else
            stationsString=$stationsString';\|'$dataStationId
        fi
    done < <(tail -n +2 $tmpFile)
    rm $tmpFile
    stationsString=$stationsString'"'
}

# create dates string to filter data
function buildDatesPattern {   
    local params
    local startDate
    local endDate
    local minTs=0
    local maxTs=0    
    local tmpTs=0    
    local tmpDate       
    local start
    local end    
    local duration    
    local now=$(date +%s)    

    echo '+ currently building dates filter'
    start=$(date +%s)
    minTs=$(date -d $dataMinDate +%s)
    maxTs=$(date -d $dataMaxDate +%s)

    for datOpt in "${dates[@]}"
    do
        IFS=';' read -r -a params <<< "$datOpt"
        # data entered by the user
        startDate="${params[1]}"
        endDate="${params[2]}"
        if [ $startDate -gt $endDate ] || [ $startDate -gt $now ]    
        then
            echo 'wrong date interval'
            sendReturnCode 1 'wrong date interval'
        fi
        if [ $endDate -lt $minTs ] || [ $startDate -gt $maxTs ]
        then
            echo 'no data for this interval'
            continue
        fi
        tmpTs=$startDate
        while [ $tmpTs -le $endDate ]
        do 
            tmpDate=$(date -d "@$tmpTs" +"%Y-%m-%d")
            if [ $i -eq 0 ]
            then
                datesString="${datesString}""${tmpDate}"
                i=1
            else
                datesString="${datesString}"$TIMESEPARATOR'\|'"${tmpDate}"            
            fi
            tmpTs=$(expr $tmpTs + $SECONDSPERDAY )            
        done
        datesString="${datesString}"$TIMESEPARATOR$QUOTEDQUOTES
        end=$(date +%s)
        duration=$(expr $end - $start)
    done    
}

# function that filters with a date delimitation
function filterWithDate {
    local inputDate=$1
    local datOpt
    local datInp=$(date '+%s' -d "$inputDate")
    local param
    local i=0
    local j=0
    local cmd=255
    local start=$(date +%s)
    local end=0
    local duration=0    
    local res1=$FALSE
    local res2=$FALSE
    local tmpSeconds=0

    getMinMaxDates
    buildDatesPattern

    cmd=$(cat $dataFile |grep "$datesString" > $timeFilteredFile)
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode 0 'no corresponding data for dates provided'
    fi
    dataFile=$timeFilteredFile    
    end=$(date +%s)
    duration=$(expr $end - $start)
    echo '--> dates filtering operation duration : ' $duration ' seconds'    

    return
}

# function that filters the places + delimitation of the zones
function filterWithLocation {   
    local cmd=255
    local start=$(date +%s)
    local end=0
    local duration=0

    echo '+ currently filtering stations'
    buildZonePattern
    cmd=$(cat $dataFile |grep "$stationsString" > $zoneFilteredFile)
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode 1 'error extracting stations'
    fi
    dataFile=$zoneFilteredFile    
    end=$(date +%s)
#    echo $start $end
    duration=$(expr $end - $start)
    echo '--> geographical zone filtering operation duration : ' $duration ' seconds'

    return  
}

# create a command line string has per user input
function buildCommandStrings {
    local i=0
    local fileId=1
    local fileNameIn=''
    local fileNameOut=''
    local option
    local reverse
    local recordsCount=0
    local pCounter=0
    local tCounter=0
    local typOpt

    for typOpt in "${types[@]}"
    do
        reverse=''
        option=${typOpt:0:2}
        if [ $option == '-h' ] || [ $option == '-m' ]
        then
            reverse='-r '
        fi
        case "${typOpt}" in 
            "-t;1") fileNameIn=$TMPDIR't1'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR't1'$FILETERMDAT
                    ;;
            "-t;2") fileNameIn=$TMPDIR't2'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR't2'$FILETERMDAT
                    ;;        
            "-t;3") fileNameIn=$TMPDIR't3'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR't3'$FILETERMDAT
                    ;;                                      
            "-p;1") fileNameIn=$TMPDIR'p1'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'p1'$FILETERMDAT
                    ;;
            "-p;2") fileNameIn=$TMPDIR'p2'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'p2'$FILETERMDAT
                    ;;
            "-p;3") fileNameIn=$TMPDIR'p3'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'p3'$FILETERMDAT
                    ;;                                        
            "-h")   fileNameIn=$TMPDIR'h'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'h'$FILETERMDAT
                    ;;
            "-m")   fileNameIn=$TMPDIR'm'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'm'$FILETERMDAT
                    ;;
            "-w")   fileNameIn=$TMPDIR'w'$FILETERMCSV
                    fileNameOut=$OUTPUTDIR'w'$FILETERMDAT
                    ;;                                                                             
        esac
        recordsCount=$(cat $fileNameIn |wc -l)
        if [ $recordsCount -eq 0 ]
        then
            continue
        fi
        echo '+' $recordsCount 'records written to' $fileNameIn
        commandStrings[$i]=${typOpt/;/ }        
        commandStrings[$i]="${commandStrings[$i]}"' -f '$fileNameIn' -o '$fileNameOut' '"$reverse"$sortingMethod       
        i=$((i+1))    
        fileId=$((fileId+1))         
    done    
}

# for debugging purpose only, display array content
function printArray {
    local item

    IFS=', ' read -r -a array <<< "$@"    
    for item in "${array[@]}"
    do
        echo $item
    done
}

# function that extracts all fields from the database
function extractData {
    local extractOption
    local extractArgument
    local extractInput
    local dataStationId
    local dataDate
    local dataSeaPressure
    local dataWindDirection
    local dataWindSpeed
    local dataMoisture
    local dataStationPressure
    local data24hPressureVariation
    local data24hRainfall
    local dataCoordinates
    local dataTemperature
    local data24hMinTemperature
    local data25hMaxTemperature
    local dataHeight
    local dataPostcode
    local fileId=1
    local fileName=''
    local line=''
    local lineId=1
    local fileLine=1
    local validRecords=0
    local linesCount=$(cat $dataFile |wc -l)
    local echoLine=''
    local locationsCount="${#locations[@]}"
    local datesCount="${#dates[@]}"
    local retval=255
    local fields=''
    local start=$(date +%s)
    local end=0
    local duration=0    

    # input file information
    # field 1 : station id
    # field 2 : date
    # field 3 : sea level pressure
    # field 4 : wind direction
    # field 5 : wind speed
    # field 6 : moisture
    # field 7 : station level pressure
    # field 8 : 24h pression variation
    # field 9 : 24H rain fall
    # field 10 : coordinates
    # field 11 : temperature
    # field 12 : 24H mininal temperature
    # field 13 : 24 maximal temperature
    # field 14 : height
    # field 15 : postcode

    echo '+ currently extracting data'
 
    for typOpt in "${types[@]}"
    do
#        fileName="$TMPDIR$fileId$FILETERMCSV"
        fileId=$((fileId+1))      
        case "${typOpt}" in 
            "-t;1") # Station id, temp, min temp, max temp
                    fields='1,11,12,13'
                    fileName=$TMPDIR't1'$FILETERMCSV
                    ;;
            "-t;2") # Date, temp
                    fileName=$TMPDIR't2'$FILETERMCSV
                    fields='2,11'
                    ;;
            "-t;3") # Station id, date, temp
                    fileName=$TMPDIR't3'$FILETERMCSV
                    fields='1,2,11'
                    ;;                                
            "-p;1") # Station id, pressure
                    fileName=$TMPDIR'p1'$FILETERMCSV
                    fields='1,7' 
                    ;;
            "-p;2") # Date, pressure
                    fileName=$TMPDIR'p2'$FILETERMCSV
                    fields='2,7'                  
                    ;;
            "-p;3") # Station id, date, pressure
                    fileName=$TMPDIR'p3'$FILETERMCSV
                    fields='1,2,7'
                    ;;                
            -w)     # Station id, wind direction, wind speed, coordinates
                    fields='1,4,5,10'                      
                    fileName=$TMPDIR'p'$FILETERMCSV
                    ;;
            -h)     # Station id, coordinates, height
                    fields='1,10,14'               
                    fileName=$TMPDIR'h'$FILETERMCSV
                    ;;  
            -m)     # Station id, moisture, coordinates
                    fields='1,6,10'                
                    fileName=$TMPDIR'm'$FILETERMCSV
                    ;;               
        esac
        cmd=$(tail -n +2 $dataFile |cut -d"$CSVSEPARATOR" -f$fields |grep -v "$EMPTYFIELDFILTER" |tr -d ':' > $fileName)
    done
    end=$(date +%s)
    duration=$(expr $end - $start)
    echo '--> data extraction operation duration : ' $duration ' seconds'    
    buildCommandStrings       
}

# compiles C program
function compileCExec {
    local retval=255
    
    $GCC -w -o meteo src/meteo.c lib/functions_common.c lib/functions_list.c \
        lib/functions_avl.c lib/functions_abr.c
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode $OTHERERROR 'unable to compile C program'
    fi
}

# executes C program with extraction, input and output info
function launchProgram {
    local retval=255
    local start=$(date +%s)
    local end=0

    for cmdStr in "${commandStrings[@]}"
    do
        echo "+ launch C program with command line : $CEXEC $cmdStr"
        $CEXEC $cmdStr
        retval=$?
        if [ $retval -gt 0 ]
        then
            sendReturnCode $retval 'error received from C program'
        fi
    done    
    end=$(date +%s)
    duration=$(expr $end - $start)
    echo '--> data sorting and graphics creation operation duration : ' $duration ' seconds'     
    echo '+ operation(s) terminated successfully'
}

# creates gnuplot graphics with data created by C program
function generateGraphics {
    local typOpt
    local option
    local digit
    local gnuplotCommand=''
    local outputFile=''
    local retval=255
    local optDig=''

    for typOpt in "${types[@]}"
    do
        option=${typOpt:1:1}
        digit=''
        if [ ${#typOpt} -gt 2 ]
        then
            digit=${typOpt:3:1}
        fi
        optDig=$option$digit
        if [ $optDig == 'p3' ] || [ $optDig == 't3' ]
        then
            addInfoToGnuplot $optDig
        fi
        gnuplotCommand=$LIBDIR$option$digit$FILETERMSH
        chmod +x $gnuplotCommand
        outputFile=$RESULTDIR$option$digit$FILETERMPNG
        $gnuplotCommand   
        retval=$?
        if [ $retval -eq 0 ]
        then
            echo '+ output to' $outputFile
        else
            echo '+ error trying to create' $outputFile
        fi
    done    
}

function addInfoToGnuplot {
    local optDig=$1
    local cmd=''
    local entry
    local toAdd=''

    if [ $optDig == 'p3' ]
    then
        cmd=$P3GNUPLOTCMD
        for entry in "$P3OUTPUTDIR"/*.dat
        do
            toAdd=', "'"$entry"'" u 1:2 w l'
            cmd=$cmd$toAdd
        done
        echo $cmd >> $LIBDIR$optDig$FILETERMSH
    fi 
    if [ $optDig == 't3' ]
    then
        cmd=$T3GNUPLOTCMD
        for entry in "$T3OUTPUTDIR"/*.dat
        do
            toAdd=', "'"$entry"'" u 1:2 w l'
            cmd=$cmd$toAdd
        done
        echo $cmd >> $LIBDIR$optDig$FILETERMSH
    fi     
}

# manage return codes and exits
function sendReturnCode {
    local returnCode=$1
    local message=$2    

    case "$returnCode" in
        0)  if ! [ -z "$message" ]
            then
                echo -n $message
                echo   
            fi    
            ;;
        1)  local option=$3
            local argument=$4
            if ! [ -z "$message" ]
            then
                echo -n $errorPattern $message    
            else
                echo -n $errorPattern $defaultError          
            fi            
            if ! [ -z "$option" ]
            then
                echo -n " $option"
            fi
            if ! [ -z "$argument" ]
            then
                echo ', [argument] =' "$argument"
            fi            
            displayHelp
            ;;
        2)  local inputFile=$3
            echo $errorPattern $message $inputFile
            ;;
        3)  local outputFile=$3
            echo $errorPattern 'unable to open output file' $outputFile
            ;;
        4)  echo $errorPattern 'internal error'
            ;;       
        *)  echo $errorPattern 'unknown error'
            ;;         
    esac
    echo
    exit $returnCode
}
