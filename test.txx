import com.ibm.mq.jms.MQQueueConnectionFactory;
import com.ibm.msg.client.wmq.WMQConstants;
import javax.jms.*;

public class MqRequestResponseExample {

    public static void main(String[] args) {
        // Connection parameters
        String queueManager = "QMGR";
        String requestQueueName = "REQUEST.QUEUE";
        String replyQueueName = "REPLY.QUEUE";
        String host = "localhost";
        int port = 1414;
        String channel = "CHANNEL_NAME";
        String user = "username";
        String password = "password";

        try {
            // Setup MQ connection factory
            MQQueueConnectionFactory factory = new MQQueueConnectionFactory();
            factory.setHostName(host);
            factory.setPort(port);
            factory.setQueueManager(queueManager);
            factory.setChannel(channel);
            factory.setTransportType(WMQConstants.WMQ_CM_CLIENT);

            // Setup JMS connection, session, and queues
            Connection connection = factory.createConnection(user, password);
            Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);

            Queue requestQueue = session.createQueue(requestQueueName);
            Queue replyQueue = session.createQueue(replyQueueName);

            // Create a producer to send request messages
            MessageProducer producer = session.createProducer(requestQueue);
            MessageConsumer consumer = session.createConsumer(replyQueue);

            // Start the connection
            connection.start();

            // Create and send request message
            TextMessage requestMessage = session.createTextMessage("Request Data");
            requestMessage.setJMSReplyTo(replyQueue); // Set reply queue for response
            producer.send(requestMessage);

            // Print the message ID for reference
            System.out.println("Sent request message ID: " + requestMessage.getJMSMessageID());

            // Set the correlation ID on the consumer for response
            String correlationID = requestMessage.getJMSMessageID();
            String messageSelector = "JMSCorrelationID = '" + correlationID + "'";
            consumer = session.createConsumer(replyQueue, messageSelector);

            // Receive the reply message
            Message replyMessage = consumer.receive(5000); // Wait up to 5 seconds

            if (replyMessage != null && replyMessage instanceof TextMessage) {
                TextMessage textReply = (TextMessage) replyMessage;
                System.out.println("Received reply: " + textReply.getText());
            } else {
                System.out.println("No reply received within the timeout period.");
            }

            // Clean up resources
            producer.close();
            consumer.close();
            session.close();
            connection.close();

        } catch (JMSException e) {
            e.printStackTrace();
        }
    }
}
