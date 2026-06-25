#include <iostream>
#include <string>
using namespace std;

// Forward declaration
class UIComponent;

// Mediator Interface
class UIMediator {
public:
    virtual void componentChanged(UIComponent* component) = 0;
    virtual ~UIMediator() {}
};

// Base Component
class UIComponent {
protected:
    UIMediator* mediator;

public:
    UIComponent(UIMediator* mediator) : mediator(mediator) {}

    void notifyMediator() {
        mediator->componentChanged(this);
    }

    virtual ~UIComponent() {}
};


// Concrete Components
class TextField : public UIComponent {
private:
    string text;

public:
    TextField(UIMediator* mediator)
        : UIComponent(mediator), text("") {}

    void setText(const string& newText) {
        text = newText;
        cout << "TextField updated: " << newText << endl;
        notifyMediator();
    }

    string getText() const {
        return text;
    }
};

class Button : public UIComponent {
private:
    bool enabled;

public:
    Button(UIMediator* mediator)
        : UIComponent(mediator), enabled(false) {}

    void click() {
        if (enabled) {
            cout << "Login Button clicked!" << endl;
            notifyMediator();
        } else {
            cout << "Login Button is disabled." << endl;
        }
    }

    void setEnabled(bool value) {
        enabled = value;
        cout << "Login Button is now "
             << (enabled ? "ENABLED" : "DISABLED") << endl;
    }
};

class Label : public UIComponent {
private:
    string text;

public:
    Label(UIMediator* mediator)
        : UIComponent(mediator) {}

    void setText(const string& message) {
        text = message;
        cout << "Status: " << text << endl;
    }
};


// Concrete Mediator
class FormMediator : public UIMediator {
private:
    TextField* usernameField;
    TextField* passwordField;
    Button* loginButton;
    Label* statusLabel;

public:
    FormMediator()
        : usernameField(nullptr),
          passwordField(nullptr),
          loginButton(nullptr),
          statusLabel(nullptr) {}

    void setUsernameField(TextField* field) {
        usernameField = field;
    }

    void setPasswordField(TextField* field) {
        passwordField = field;
    }

    void setLoginButton(Button* button) {
        loginButton = button;
    }

    void setStatusLabel(Label* label) {
        statusLabel = label;
    }

    void componentChanged(UIComponent* component) override {

        if (component == usernameField || component == passwordField) {

            bool enableButton =
                !usernameField->getText().empty() &&
                !passwordField->getText().empty();

            loginButton->setEnabled(enableButton);
        }
        else if (component == loginButton) {

            string username = usernameField->getText();
            string password = passwordField->getText();

            if (username == "admin" && password == "1234") {
                statusLabel->setText("Login successful!");
            } else {
                statusLabel->setText("Invalid credentials.");
            }
        }
    }
};


// Client
class MediatorApp {
public:
    static void main() {

        FormMediator mediator;

        TextField usernameField(&mediator);
        TextField passwordField(&mediator);
        Button loginButton(&mediator);
        Label statusLabel(&mediator);

        mediator.setUsernameField(&usernameField);
        mediator.setPasswordField(&passwordField);
        mediator.setLoginButton(&loginButton);
        mediator.setStatusLabel(&statusLabel);

        // First Attempt
        usernameField.setText("admin");
        passwordField.setText("1234");
        loginButton.click();

        cout << "\n--- New Attempt with Wrong Password ---\n";

        // Second Attempt
        passwordField.setText("wrong");
        loginButton.click();
    }
};

int main() {
    MediatorApp::main();
    return 0;
}