import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class exercise_12_11 {
    public static void main(String[] args) throws FileNotFoundException {
        File sourceFile = new File(args[1]);
        //判断文件是否存在
        if (!sourceFile.exists()) {
            System.out.println("File don't exist.");
        }
        //创建一个空的字符串构建器,sb的对象为StringBuilder类
        Scanner input = new Scanner(sourceFile);
        StringBuilder sb = new StringBuilder();
        //在还有内容可以读的时候,不停地读入下一行
        while (input.hasNext()) {
            String line = input.nextLine();
            sb.append(line.replaceAll(args[0], "") + "\r\n");
        }
        input.close();

        //再把内容从构建器中输出出来
        PrintWriter output = new PrintWriter(sourceFile);
        //必须使用toString()方法才可以将构建器中的字符序列输出给新的String类对象
        output.println(sb.toString());
        output.close();
    }
}