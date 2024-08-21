package kernel;

import java.util.Date;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;;

public class kernellogformat extends Formatter
{
    
    @Override
    public synchronized String format(LogRecord record) {
        return String.format(
            DEFAULT_FORMAT, 
            new Date(record.getMillis()), 
            record.getLoggerName(), 
            record.getLevel().getLocalizedName(),
            formatMessage(record)
        );
    } // format 

    private static final String DEFAULT_FORMAT = "%1$tH:%1$tM:%1$tS %2$s: %3$s: %4$s%n";
    
} // public class kernel log format
 