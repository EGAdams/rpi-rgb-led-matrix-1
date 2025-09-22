# Rename all *.uno to *.cpp
for f in *.ino; do
    mv -- "$f" "${f%.ino}.cpp"
done