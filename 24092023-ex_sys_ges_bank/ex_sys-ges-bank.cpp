#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>

class BankAccount {
private:
    static int next_account_number;
    std::string account_number;
    std::string first_name;
    std::string last_name;
    double balance;
    std::time_t creation_time;

public:
    BankAccount(const std::string& first_name, const std::string& last_name, double initial_balance)
        : first_name(first_name), last_name(last_name), balance(initial_balance) {
        generateAccountNumber();
        creation_time = std::time(nullptr);

        // Création d'un fichier texte pour chaque compte
        std::ofstream account_file(first_name + last_name + ".txt");
        if (account_file.is_open()) {
            account_file << "Titulaire du compte : " << first_name << " " << last_name << std::endl;
            account_file << "Mot de passe : " << generatePassword() << std::endl;

            std::tm local_time = {};
            localtime_s(&local_time, &creation_time);
            char buffer[26];
            asctime_s(buffer, sizeof(buffer), &local_time);

            account_file << "Heure de création du compte : " << buffer;
            account_file.close();
        }
    }

    const std::string& getAccountNumber() const {
        return account_number;
    }

    void displayAccountDetails() const {
        std::cout << "\n***********************************************\n";
        std::cout << "                Détails du compte\n";
        std::cout << "***********************************************\n";
        std::cout << "Numéro de compte : " << account_number << std::endl;
        std::cout << "Titulaire du compte : " << first_name << " " << last_name << std::endl;
        std::cout << "Solde du compte : " << std::fixed << std::setprecision(2) << balance << " €" << std::endl;

        std::tm local_time = {};
        localtime_s(&local_time, &creation_time);
        char buffer[26];
        asctime_s(buffer, sizeof(buffer), &local_time);
        std::cout << "Heure de création du compte : " << buffer;
        std::cout << "***********************************************\n";
    }

    void deposit(double amount) {
        balance += amount;
        std::cout << "\n***********************************************\n";
        std::cout << "               Opération de dépôt\n";
        std::cout << "***********************************************\n";
        std::cout << "Dépôt de " << std::fixed << std::setprecision(2) << amount << " € effectué avec succès.\n";
        std::cout << "Nouveau solde : " << balance << " €\n";
        std::cout << "***********************************************\n";
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            std::cout << "\n***********************************************\n";
            std::cout << "               Opération de retrait\n";
            std::cout << "***********************************************\n";
            std::cout << "Retrait de " << std::fixed << std::setprecision(2) << amount << " € effectué avec succès.\n";
            std::cout << "Nouveau solde : " << balance << " €\n";
            std::cout << "***********************************************\n";
        }
        else {
            std::cout << "\n***********************************************\n";
            std::cout << "              Opération de retrait\n";
            std::cout << "***********************************************\n";
            std::cout << "Fonds insuffisants pour le retrait.\n";
            std::cout << "Solde actuel : " << balance << " €\n";
            std::cout << "***********************************************\n";
        }
    }

private:
    void generateAccountNumber() {
        std::srand(static_cast<unsigned>(std::time(0)));
        const std::string alphanumeric_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const int length = 8;

        for (int i = 0; i < length; ++i) {
            account_number += alphanumeric_chars[std::rand() % alphanumeric_chars.length()];
        }
    }

    std::string generatePassword() {
        const std::string alphanumeric_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const int length = 10;
        std::string password;

        for (int i = 0; i < length; ++i) {
            password += alphanumeric_chars[std::rand() % alphanumeric_chars.length()];
        }

        return password;
    }
};

int BankAccount::next_account_number = 1000;

void displayMenu() {
    std::cout << "\n***********************************************\n";
    std::cout << "                 Menu interactif\n";
    std::cout << "***********************************************\n";
    std::cout << "1. Afficher les détails d'un compte\n";
    std::cout << "2. Effectuer un dépôt\n";
    std::cout << "3. Effectuer un retrait\n";
    std::cout << "4. Créer un nouveau compte\n";
    std::cout << "5. Quitter\n";
    std::cout << "***********************************************\n";
    std::cout << "Choisissez une option (1-5) : ";
}

int main() {
    std::vector<BankAccount> accounts;

    char choice;

    do {
        displayMenu();
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case '1': {
            std::string account_number;
            std::cout << "Entrez le numéro de compte : ";
            std::cin >> account_number;

            bool found = false;
            for (const auto& account : accounts) {
                if (account_number == account.getAccountNumber()) {
                    found = true;
                    account.displayAccountDetails();
                    break;
                }
            }

            if (!found) {
                std::cout << "\n***********************************************\n";
                std::cout << "Aucun compte trouvé avec le numéro spécifié.\n";
                std::cout << "***********************************************\n";
            }

            break;
        }
        case '2': {
            std::string account_number;
            std::cout << "Entrez le numéro de compte : ";
            std::cin >> account_number;

            double amount;
            std::cout << "Entrez le montant du dépôt : ";
            std::cin >> amount;

            for (auto& account : accounts) {
                if (account_number == account.getAccountNumber()) {
                    account.deposit(amount);
                    break;
                }
            }

            break;
        }
        case '3': {
            std::string account_number;
            std::cout << "Entrez le numéro de compte : ";
            std::cin >> account_number;

            double amount;
            std::cout << "Entrez le montant du retrait : ";
            std::cin >> amount;

            for (auto& account : accounts) {
                if (account_number == account.getAccountNumber()) {
                    account.withdraw(amount);
                    break;
                }
            }

            break;
        }
        case '4': {
            std::cin.ignore();
            std::string first_name;
            std::string last_name;
            double initial_balance;

            std::cout << "Entrez le prénom du titulaire du compte : ";
            std::getline(std::cin, first_name);

            std::cout << "Entrez le nom du titulaire du compte : ";
            std::getline(std::cin, last_name);

            while (true) {
                std::cout << "Entrez le solde initial du compte : ";
                if (std::cin >> initial_balance) {
                    break;
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Entrée invalide. Veuillez entrer un nombre valide.\n";
                }
            }

            accounts.push_back(BankAccount(first_name, last_name, initial_balance));
            std::cout << "\n***********************************************\n";
            std::cout << "Nouveau compte créé avec succès. Numéro de compte : "
                << accounts.back().getAccountNumber() << std::endl;
            std::cout << "***********************************************\n";

            break;
        }
        case '5':
            std::cout << "\n***********************************************\n";
            std::cout << "                Au revoir !\n";
            std::cout << "***********************************************\n";
            break;
        default:
            std::cout << "\n***********************************************\n";
            std::cout << "Option invalide. Veuillez choisir une option valide.\n";
            std::cout << "***********************************************\n";
        }

    } while (choice != '5');

    return 0;
}
