import java.util.Arrays;

class LogParser {
    public static void main(String[] args) {
        String S1 = "abc dfg";
        System.out.println(S1);

        S1 = S1.replace(" ", "\t");
        System.out.println(S1);
    }

    /**
     * 解析日志的行记录
     *
     * @param line
     * @return 字符串数组:ip,mail,status
     */
    public String[] parse(String line) {
        String id = parseID(line);
        String mail = parseMail(line);
        String status = parseStatus(line);
        return new String[]{id, mail, status};
    }

    public String parseID(String line) {
        int index = line.indexOf("delivery");
        if (index != -1) {
            String id = line.substring(index + "delivery ".length(), line.indexOf(":"));
            return id;
        }
        return null;
    }

    public String parseMail(String line) {
        int index = line.indexOf("remote");
        if (index != -1) {
            String mail = line.substring(index + "remote ".length());
            return mail;
        }
        return null;
    }

    public String parseStatus(String line) {
        String tmp = line.substring(line.indexOf(":") + 1);
        int index = tmp.indexOf(": ");
        if (index != -1) {
            String status = tmp.substring(1, index);
            System.out.println("<" + status + ">");
            if ("success".equals(status) || "failure".equals(status) || "deferral".equals(status)) {
                return status;
            }
        }
        return null;
    }
}