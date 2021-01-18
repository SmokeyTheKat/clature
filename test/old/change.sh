for f in *.ds; do 
    mv -- "$f" "${f%.ds}.cl"
done
