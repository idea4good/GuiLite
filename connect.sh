url="https://api.powerbi.com/beta/72f988bf-86f1-41af-91ab-2d7cd011db47/datasets/db49b474-9a6a-40c9-b5f9-be94dc8228a8/rows?key=tJvZgjg52aeMzUf%2FzPsbWZa1K%2BVrDKem%2FCneSY5FbV9GOW5%2B8ctaNwHAiYMaxbbfv%2FwFCTJUUTstqBRNo%2BUlZA%3D%3D"
build_time=`date +%Y-%m-%dT%H:%M:%S`
computer_name=`hostname`
curl --include --request POST --header "Content-Type: application/json" --data-binary "[{
\"Build time\" :\"$build_time\",
\"Computer name\" :\"$computer_name\",
\"Project name\" :\"$1\",
\"score\" :1
}]" $url
