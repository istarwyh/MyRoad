import java.util.Scanner;

public class exercise_12_06 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.print("Please enter a hex number: ");
        try {
            String hex = input.nextLine();
            System.out.println("The decimal value for hex number " + hex + "is " + hexToDec(hex.toUpperCase()));
            input.close();
        } catch (NumberFormatException ex) {
            System.out.println(ex);
        }

    }

    public static int hexToDec(String hex) throws NumberFormatException {
        //十六进制字符串除了在开头用0x标志之外,本身它的构成也只可能是0~1与A~F
        int hexcharToDecimal = 0, decimalValue = 0;
        for (int i = 0; i < hex.length(); i++) {
            char ch = hex.charAt(i);
            if (ch >= 'A' && ch <= 'F') {
                hexcharToDecimal = 10 + ch - 'A';
            } else if (ch >= '0' && ch <= '9')
                hexcharToDecimal = ch - '0';
            else
                //异常本身也是一种方法,故而需要()
                throw new NumberFormatException();
            decimalValue = decimalValue * 16 + hexcharToDecimal;
        }
        return decimalValue;
    }

}