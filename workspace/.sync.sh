if [ "$#" -ne 1 ]; then
    echo "Invalid arguments"
    exit -1
fi

url="https://api.powerbi.com/beta/72f988bf-86f1-41af-91ab-2d7cd011db47/datasets/2ff1e8a8-2f6f-4d73-a75d-86829e3f4574/rows?key=8f5xLp1gP8%2FzSee4vCUBcyjR65I9zZ6nb%2B%2F7bbzex%2FSctLX3ntIlAR0sxWpDdguuYyDtLdHK%2Fxbxj%2FrSBkX7eQ%3D%3D"
build_time=`date +%Y-%m-%dT%H:%M:%S.000%z`
device_info=`uname -s -n -m`

#--------------- Geo info ----------------#
curl ipinfo.io > ip_info.txt # get IP info

grep city ip_info.txt > ip_city.txt # filter city
sed -i 's/"city"://g' ip_city.txt #remove property name
sed -i 's/"//g' ip_city.txt #remove double quotes
city=`sed 's/,//g' ip_city.txt` #remove comma

grep country ip_info.txt > ip_country.txt # filter country
sed -i 's/"country"://g' ip_country.txt #remove property name
sed -i 's/"//g' ip_country.txt #remove double quotes
country=`sed 's/,//g' ip_country.txt` #remove comma

grep org ip_info.txt > ip_org.txt # filter org
sed -i 's/"org"://g' ip_org.txt #remove property name
sed -i 's/"//g' ip_org.txt #remove double quotes
org=`sed 's/,//g' ip_org.txt` #remove comma

curl --include --request POST --header "Content-Type: application/json" --data-binary "[{
\"device_info\" :\"$device_info\",
\"project_info\" :\"$1\",
\"time\" :\"$build_time\",
\"weight\" :1,
\"country\" :\"$country\",
\"city\" :\"$city\",
\"org\" :\"$org\"
}]" $url > /dev/null

rm ip_*
