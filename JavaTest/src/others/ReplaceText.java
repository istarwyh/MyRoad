import java.io.*;
import java.util.*;

//这个适合读入然后输出到另一个文本文件的情况
public class ReplaceText {
    public static void main(String[] args) throws FileNotFoundException {
        //check command line parameter usgae
        if (args.length != 4) {
            System.out.println("Usage: java RepalceText sourceFile targetFile oldStr newStr");
            System.exit(1);
        }
        File sourceFile = new File(args[0]);
        if (!sourceFile.exists()) {
            System.out.println("Source file" + args[0] + " does not exist");
            System.exit(2);
        }
        File targetFile = new File(args[1]);
        if (targetFile.exists()) {
            System.out.println("Target file" + args[1] + "already exists");
            System.exit(3);
        }
        try (
                //这里的输入流必须是源文件的啊!
                Scanner input = new Scanner(sourceFile);
                PrintWriter output = new PrintWriter(targetFile);
        ) {
            while (input.hasNext()) {
                //如果这里是input.next()的话,将会以空格为间隔读入然后输出,这里通过控制格式输出或许可以做点文章
                String line = input.nextLine();
                String line2 = line.replaceAll(args[2], args[3]);
                output.println(line2);
                //通过读入一行再输出一行可以保留原文格式,实现删除要求
            }
        }
    }
}