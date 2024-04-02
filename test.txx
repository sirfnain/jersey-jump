import java.time.DayOfWeek;
import java.time.LocalDateTime;

public class YesterdayCalculator {

    public static DayOfWeek determineYesterday(LocalDateTime currentDateTime) {
        DayOfWeek yesterday = null;
        DayOfWeek currentDay = currentDateTime.getDayOfWeek();
        int currentHour = currentDateTime.getHour();
        int currentMinute = currentDateTime.getMinute();

        if ((currentDay == DayOfWeek.FRIDAY && (currentHour > 8 || (currentHour == 8 && currentMinute >= 30))) ||
            (currentDay == DayOfWeek.SATURDAY) ||
            (currentDay == DayOfWeek.SUNDAY && (currentHour < 8 || (currentHour == 8 && currentMinute < 30)))) {
            yesterday = DayOfWeek.THURSDAY;
        } else if ((currentDay == DayOfWeek.SUNDAY && (currentHour > 8 || (currentHour == 8 && currentMinute >= 30))) ||
                   (currentDay == DayOfWeek.MONDAY) ||
                   (currentDay == DayOfWeek.TUESDAY && (currentHour < 8 || (currentHour == 8 && currentMinute < 30)))) {
            yesterday = DayOfWeek.FRIDAY;
        } else if ((currentDay == DayOfWeek.TUESDAY && (currentHour > 8 || (currentHour == 8 && currentMinute >= 30))) ||
                   (currentDay == DayOfWeek.WEDNESDAY) ||
                   (currentDay == DayOfWeek.THURSDAY && (currentHour < 8 || (currentHour == 8 && currentMinute < 30)))) {
            yesterday = DayOfWeek.MONDAY;
        } else if ((currentDay == DayOfWeek.WEDNESDAY && (currentHour > 8 || (currentHour == 8 && currentMinute >= 30))) ||
                   (currentDay == DayOfWeek.THURSDAY) ||
                   (currentDay == DayOfWeek.FRIDAY && (currentHour < 8 || (currentHour == 8 && currentMinute < 30)))) {
            yesterday = DayOfWeek.TUESDAY;
        }

        return yesterday;
    }

    public static void main(String[] args) {
        // Test the logic
        LocalDateTime currentDateTime = LocalDateTime.now();
        DayOfWeek yesterday = determineYesterday(currentDateTime);
        System.out.println("Yesterday was: " + yesterday);
    }
}
