package com.example.validation;

public class ValidationResult {
//test commit
    private final boolean retry;
    private final boolean reprocess;

    public ValidationResult(boolean retry, boolean reprocess) {
        this.retry = retry;
        this.reprocess = reprocess;
    }

    public boolean isRetry() {
        return retry;
    }

    public boolean isReprocess() {
        return reprocess;
    }

    @Override
    public String toString() {
        return "ValidationResult{" +
            "retry=" + retry +
            ", reprocess=" + reprocess +
            '}';
    }
}


------------------------------
package com.example.validation;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

@Service
public class ValidationService {

    private static final Logger logger = LoggerFactory.getLogger(ValidationService.class);

    private final ValidationProperties validationProperties;

    private final List<Pattern> retryPatterns = new ArrayList<>();
    private final List<Pattern> reprocessPatterns = new ArrayList<>();

    public ValidationService(ValidationProperties validationProperties) {
        this.validationProperties = validationProperties;
    }

    @PostConstruct
    private void initializePatterns() {
        // Compile retry patterns
        for (String regex : validationProperties.getRetry()) {
            retryPatterns.add(Pattern.compile(regex));
        }

        // Compile reprocess patterns
        for (String regex : validationProperties.getReprocess()) {
            reprocessPatterns.add(Pattern.compile(regex));
        }

        logger.info("Validation patterns initialized. Retry: {}, Reprocess: {}", retryPatterns, reprocessPatterns);
    }

    public ValidationResult validateInput(String input) {
        logger.info("Validating input: {}", input);

        boolean retry = matchesAnyPattern(input, retryPatterns);
        boolean reprocess = matchesAnyPattern(input, reprocessPatterns);

        return new ValidationResult(retry, reprocess);
    }

    private boolean matchesAnyPattern(String input, List<Pattern> patterns) {
        for (Pattern pattern : patterns) {
            if (pattern.matcher(input).matches()) {
                logger.debug("Input '{}' matches pattern: {}", input, pattern);
                return true;
            }
        }
        return false;
    }
}

---------------

package com.example.validation;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
@ConfigurationProperties(prefix = "validation.regex")
public class ValidationProperties {

    private List<String> retry;
    private List<String> reprocess;

    public List<String> getRetry() {
        return retry;
    }

    public void setRetry(List<String> retry) {
        this.retry = retry;
    }

    public List<String> getReprocess() {
        return reprocess;
    }

    public void setReprocess(List<String> reprocess) {
        this.reprocess = reprocess;
    }
}
------------
# Retry regex patterns
validation.regex.retry.pattern1=^failed to find curve [a-zA-Z0-9_]+ and [a-zA-Z0-9_]+$
validation.regex.retry.pattern2=^Database error [a-zA-Z0-9_]+ and [a-zA-Z0-9_]+ and [a-zA-Z0-9_]+$

# Reprocess regex patterns
validation.regex.reprocess.pattern3=^rectifying failed for [a-zA-Z0-9_]+ and [a-zA-Z0-9_]+$
validation.regex.reprocess.pattern4=^unable to find [a-zA-Z0-9_]+ with [a-zA-Z0-9_]+$

