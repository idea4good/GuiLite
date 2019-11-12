echo off
set argC=0
for %%x in (%*) do Set /A argC+=1
if NOT "1" == "%argC%" (
    echo "Invalidate arguments"
    goto :eof
    )

set url="https://api.powerbi.com/beta/72f988bf-86f1-41af-91ab-2d7cd011db47/datasets/2ff1e8a8-2f6f-4d73-a75d-86829e3f4574/rows?key=8f5xLp1gP8%%2FzSee4vCUBcyjR65I9zZ6nb%%2B%%2F7bbzex%%2FSctLX3ntIlAR0sxWpDdguuYyDtLdHK%%2Fxbxj%%2FrSBkX7eQ%%3D%%3D"

for /f "tokens=2-4 delims=/ "  %%a in ("%date%") do (set MM=%%a& set DD=%%b& set YYYY=%%c)
set YY=%YYYY:~0,2%
if not "%YY%" == "20" (rem For Chinese date format
    for /f "tokens=1-3 delims=/ "  %%a in ("%date%") do (set YYYY=%%a& set MM=%%b& set DD=%%c))

set datetime=%YYYY%-%MM%-%DD%T%time: =0%0+0800
set devie_info=Win-%USERNAME%
set raw_data=[{^
\"device_info\" :\"%devie_info%\",^
\"project_info\" :\"%1\",^
\"time\" :\"%datetime%\",^
\"weight\" : 1^
}]

curl.exe --include --request POST --header "Content-Type: application/json" --data-binary^
 "%raw_data%" "%url%"

exit /B 0