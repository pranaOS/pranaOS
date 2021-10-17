/* switch statement example */

let vegetable = "peper"

switch vegetable {
case "celery":
    print("Add some peper to celery")
case "cucumber":
    print("Add some celery and peper to it")
case let x where x.hasSuffix("peper"):
    print("Is it a spicy \(x)?")
default:
    print("Everything tastes good in a soup.")
}