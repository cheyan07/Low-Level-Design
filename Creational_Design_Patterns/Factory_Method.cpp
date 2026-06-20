#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

using namespace std;

// Product interface
class Notification {
public:
    virtual void send(const string& message) = 0;
    virtual ~Notification() {}
};

// Concrete Products
class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending email: " << message << endl;
    }
};

class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending SMS: " << message << endl;
    }
};

class PushNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending push notification: " << message << endl;
    }
};

class SlackNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending Slack message: " << message << endl;
    }
};



// Abstract Creator
class NotificationCreator {
public:
    // Factory Method - subclasses decide what to create
    virtual unique_ptr<Notification> createNotification() = 0;

    // Shared logic that uses the factory method
    void send(const string& message) {
        auto notification = createNotification();
        notification->send(message);
    }

    virtual ~NotificationCreator() = default;
};



// Concrete Creators
class EmailNotificationCreator : public NotificationCreator {
public:
    unique_ptr<Notification> createNotification() override {
        return make_unique<EmailNotification>();
    }
};

class SMSNotificationCreator : public NotificationCreator {
public:
    unique_ptr<Notification> createNotification() override {
        return make_unique<SMSNotification>();
    }
};

class PushNotificationCreator : public NotificationCreator {
public:
    unique_ptr<Notification> createNotification() override {
        return make_unique<PushNotification>();
    }
};

class SlackNotificationCreator : public NotificationCreator {
public:
    unique_ptr<Notification> createNotification() override {
        return make_unique<SlackNotification>();
    }
};

//Client
int main() {
    // Send Email
    unique_ptr<NotificationCreator> creator = make_unique<EmailNotificationCreator>();
    creator->send("Welcome to our platform!");

    // Send SMS
    creator = make_unique<SMSNotificationCreator>();
    creator->send("Your OTP is 123456");

    // Send Push Notification
    creator = make_unique<PushNotificationCreator>();
    creator->send("You have a new follower!");

    // Send Slack Message
    creator = make_unique<SlackNotificationCreator>();
    creator->send("Standup in 10 minutes!");

    return 0;
}

// main()
//  |
//  |
//  v
// make_unique<EmailNotificationCreator>()
//  |
//  |
//  v
// EmailNotificationCreator object
//  |
//  |
//  v
// creator->send()
//  |
//  |
//  v
// NotificationCreator::send()
//  |
//  |
//  v
// createNotification()
//  |
//  | (virtual dispatch)
//  |
//  v
// EmailNotificationCreator::createNotification()
//  |
//  |
//  v
// make_unique<EmailNotification>()
//  |
//  |
//  v
// EmailNotification object
//  |
//  |
//  v
// notification->send()
//  |
//  | (virtual dispatch)
//  |
//  v
// EmailNotification::send()
//  |
//  |
//  v
// Print message

// When main() creates an EmailNotificationCreator, it stores it as a NotificationCreator pointer. 
// Calling creator->send() executes the common workflow defined in the abstract NotificationCreator. 
// Inside this workflow, the virtual factory method createNotification() is called. 
// Since the actual object is an EmailNotificationCreator, its overridden createNotification() runs and creates an EmailNotification object.
//  The returned object is stored as a Notification pointer, and when notification->send() is called, 
//  virtual dispatch invokes EmailNotification::send(). 
//  Thus, the abstract creator defines the workflow, the concrete creator decides which product to create, 
//  and the concrete product performs the actual work. In short:

// main()
//  ↓
// Concrete Creator (EmailNotificationCreator)
//  ↓
// Abstract Creator's send() workflow
//  ↓
// Concrete Creator's createNotification()
//  ↓
// Concrete Product (EmailNotification)
//  ↓
// EmailNotification::send()