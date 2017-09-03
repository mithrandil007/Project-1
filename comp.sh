gcc assembler-skeleton.c -o assembler
for i in {1..25};do
./assembler $i.s $i.exe;
diff out.exe /home/mithrandir/Desktop/tests/$i.exe;
done

