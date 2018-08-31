url="https://api.powerbi.com/beta/72f988bf-86f1-41af-91ab-2d7cd011db47/datasets/f56a3240-7d0c-45aa-a694-65ebf6a04cc2/rows?key=RHtUJLk6Vi7QIlOBAPENJuCHOT6WbVdBi%2Fa%2BhwoDFpaOFhTsPH8zXWE%2F%2FFSjPdxF7Lmkf%2Fl1q2nLgBYk7HYqUQ%3D%3D"
build_time=`date +%Y-%m-%dT%H:%M:%S`
device_info=`uname -s -n -m`
curl --include --request POST --header "Content-Type: application/json" --data-binary "[{
\"Build time\" :\"$build_time\",
\"Device info\" :\"$device_info\",
\"Project info\" :\"$1\",
\"Weight\" :1
}]" $url
