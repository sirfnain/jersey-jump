<build>
        <plugins>
            <!-- Spring Boot Maven Plugin to create the fat JAR -->
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
            <!-- Maven Assembly Plugin to create the zip distribution -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-assembly-plugin</artifactId>
                <version>3.3.0</version>
                <configuration>
                    <descriptors>
                        <descriptor>src/assembly/zip.xml</descriptor>
                    </descriptors>
                </configuration>
                <executions>
                    <execution>
                        <id>make-assembly</id>
                        <phase>package</phase>
                        <goals>
                            <goal>single</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>
---------------
<assembly xmlns="http://maven.apache.org/plugins/maven-assembly-plugin/assembly/1.1.3"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/plugins/maven-assembly-plugin/assembly/1.1.3 http://maven.apache.org/xsd/assembly-1.1.3.xsd">
    <id>zip</id>
    <formats>
        <format>zip</format>
    </formats>
    <includeBaseDirectory>false</includeBaseDirectory>
    <fileSets>
        <!-- Include the fat JAR -->
        <fileSet>
            <directory>${project.build.directory}</directory>
            <includes>
                <include>${project.build.finalName}.jar</include>
            </includes>
        </fileSet>
        <!-- Include the cfg directory -->
        <fileSet>
            <directory>src/main/resources/cfg</directory>
            <outputDirectory>cfg</outputDirectory>
        </fileSet>
    </fileSets>
</assembly>

---------------------------------------------------------
    <build>
        <plugins>
            <!-- Spring Boot Maven Plugin to create the fat JAR -->
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <executions>
                    <execution>
                        <goals>
                            <goal>repackage</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>



------------
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.event.ContextClosedEvent;
import org.springframework.context.event.EventListener;

@Configuration
public class ShutdownConfig {

    @Bean
    public GracefulShutdown gracefulShutdown(CountDownLatch latch) {
        return new GracefulShutdown(latch);
    }

    public static class GracefulShutdown {

        private final CountDownLatch latch;

        public GracefulShutdown(CountDownLatch latch) {
            this.latch = latch;
        }

        @EventListener
        public void onApplicationEvent(ContextClosedEvent event) {
            System.out.println("Received ContextClosedEvent. Initiating graceful shutdown...");
            latch.countDown();  // Release the latch to allow the application to shut down
        }
    }
}

@Bean
    public CountDownLatch latch() {
        return new CountDownLatch(1);
    }





import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.ApplicationContext;
import org.springframework.test.context.junit.jupiter.SpringExtension;

import java.util.concurrent.CountDownLatch;

import static org.junit.jupiter.api.Assertions.assertTrue;

@ExtendWith(SpringExtension.class)
@SpringBootTest
public class ApplicationTest {

    @Autowired
    private ApplicationContext context;

    @Autowired
    private CountDownLatch latch;

    @BeforeEach
    public void setUp() {
        // Set up before each test
    }

    @AfterEach
    public void tearDown() {
        // Release the latch after each test to ensure the application can shut down
        latch.countDown();
    }

    @Test
    public void testApplication() throws Exception {
        // Simulate some work in the test
        Thread.sleep(3000);  // Simulate work for 3 seconds
        assertTrue(true);  // Dummy assertion
    }
}
