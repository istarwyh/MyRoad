import java.util.Scanner;

public class exercise_12_08 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.print("Please enter a hex number: ");
        try {
            String hex = input.nextLine();
            System.out.println("The decimal value for hex number " + hex + " is " + hex2Dec(hex.toUpperCase()));
            input.close();
        } catch (HexFormatException ex) {
            //如果是NumberFormatException ex,ex即为java.lang.NumberFormatException,ex.getMessage()无内容
            //对于自定义的异常类,ex为HexFormatException: HexFormatException,ex.getMessage()有内容即HexFormatException
            System.out.println(ex.getMessage());
        }

    }

    public static int hex2Dec(String hex) throws HexFormatException {
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
                throw new HexFormatException();
            decimalValue = decimalValue * 16 + hexcharToDecimal;
        }
        return decimalValue;
    }

}

class HexFormatException extends NumberFormatException {
    /**
     *
     */
    private static final long serialVersionUID = 1L;

    public HexFormatException() {
        // System.out.println("HexFormatException");
        super("HexFormatException");
    }
}