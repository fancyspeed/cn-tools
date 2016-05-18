
import java.net.URL;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URLDecoder;
import java.net.URLEncoder;

public class EncodingUtil {
    public String[] coding_list = {"UTF-8", "GBK"};

    public String detect_encode(String str) {
        String q = str.toLowerCase();
        for (String coding : coding_list) {
            String str2 = "";
            try {
                str2 = URLDecoder.decode(str, coding).toLowerCase();
                if (q.equals(str2)) {
                    return coding;
                }
                str2 = URLEncoder.encode(str2, coding).toLowerCase();
                if (q.equals(str2) || q.equals(str2.replaceAll("\\+", "%20"))) {
                    return coding;
                }
            } catch (UnsupportedEncodingException e) {
            } catch (java.lang.IllegalArgumentException e2) {
            }
        }
        return "";
    }
    public String normalize_encoding(String str) {
        if (str == null || str.isEmpty())
            return "";
        str = str.toLowerCase();
        if (str.contains("%u")) {
            str = EscapeUtil.unescape(str);
        }
        int first_idx = str.indexOf("%");
        if (first_idx < 0)
            return str;
        if (first_idx > 0)
            return str.substring(0, first_idx) + normalize_encoding(str.substring(first_idx));
        int second_idx = first_idx;
        while (str.length() >= second_idx + 3 && str.charAt(second_idx) == '%')
            second_idx += 3;
        if (second_idx == first_idx)
            return "";

        String q = str.substring(0, second_idx).toLowerCase();
        String coding = detect_encode(q);
        if (!(coding.isEmpty())) {
            try {
                q = URLDecoder.decode(q, coding);
                q = URLDecoder.decode(q, coding);
            } catch (UnsupportedEncodingException e) {
            } catch (java.lang.IllegalArgumentException e2) {
            }
        }
        if (second_idx >= str.length())
            return q;
        return q + normalize_encoding(str.substring(second_idx));
    }


    public static void main(String args[]) {
        EncodingUtil util = new EncodingUtil();

        String[] urls = {"%E6%83%A0%E6%99%AEcompaq%208",
                "%BF%D5%C6%F8%BE%BB%BB%AF%C6%F7",
                "%E5%A8%81%E6%88%88",
                "MACBOOK%20pro",
                "%C6%BD%B0%E5%B5%E7%C4%D4",
                "%B9%F1",
                "%D4%D0%B8%BE",
                "%C8%FD%CF%BF",
                "c++",
                "/search_video/q_%E5%A5%A5%E5%B7%B4%E9%A9%AC%E6%BC%94%E8%AE%B2",
                "%BC%D9%C1%BD%BC%FE%B4%F8%B5%D7%BF%E3",
                "老西门新苑",
                "丰田卡斯特",
                "%u5B89%u8054%u9752%u5E74%u57CE%u79DF%u623F",
                "%E7%81%AB%E6%98%9F%20%E7%94%9F%E5%91%BD"};

        String q = "";
        for (String str_url : urls) {
            try {
                URL url = new URL("http://a.b?q=" + str_url);
                String qs = url.getQuery();
                q = qs.substring(qs.indexOf("=")+1).toLowerCase();
                System.out.println(String.format("url:%s, query:%s", url, q));
            } catch (MalformedURLException e) {
                continue;
            }
            String result = util.normalize_encoding(q);
            System.out.println(String.format("!    result: %s!", result));
        }

        try {
            String test = "12,34 45/78_90";
            String test2 = URLEncoder.encode(test, "UTF-8").toLowerCase();
            String test3 = URLDecoder.decode(test, "UTF-8").toLowerCase();
            System.out.println("origin: " + test);
            System.out.println("encode: " + test2);
            System.out.println("decode: " + test3);
        } catch (UnsupportedEncodingException e) {
        }
    }
}
