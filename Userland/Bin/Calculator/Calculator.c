#include <stdio.h>

void banner();

int main() {
    /**
     * @brief construct some variables
     * 
     */
    char operator;
    float num1;
    float num2;
    float result = 0;

    /**
     * @brief get user input
     * 
     */
    // printf("Basic Calculator For pranaOS\n");
    // printf("Operators: [+, *, -, /]");
    // printf("\nEnter any one operator: ");
    banner();
    scanf("%c", &operator);
    printf("Enter values: ");
    scanf("%f%f", &num1, &num2);

    /**
     * @brief Construct a new switch object for handling many operators
     * 
     */
    switch(operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        
        default:
            printf("Invalid operator!!");
    }

    printf("The value: %f", result);

    return 0;
}

void banner() {
    printf("Basic Calculator For pranaOS\n");
    printf("Operators: [+, *, -, /]");
    printf("\nEnter any one operator: ");
}