#include "Keywords.h"
#include "String.h"
#include "Delegate.h"

#include "lab.h"

bool PersonEquals(Person a, Person b)
{
    return equals(a,b);
}

size_t PersonGetHashCode(Person obj)
{
    size_t name_hash = StringComparer.Ordinal.GetHashCode(obj.Name);
    return obj.PhoneNumber ^ name_hash;
}

TAG(IEqualityComparer(Person)) PersonComparer[1] = {(TAG(IEqualityComparer(Person))) {
    .Equals = PersonEquals,
    .GetHashCode = PersonGetHashCode,
}};

HashSet(Person) GenerareHash()
{
    return new(HashSet(Person))(PersonComparer);
}

static bool string_Prefix(string source, string prefix)
{
    for (int i = 0; prefix[i]; ++i) {
        if (source[i] != prefix[i]) return false;
    }
    return true;
}

int NumaraPersoane(HashSet(Person) h, string prefixNume)
{
    int count = 0;
    foreach (var person in h) {
        if (string_Prefix(person.Name, prefixNume)) {
            count += 1;
        }
    }
    return count;
}

HashSet(Person) genereazaHashCond(HashSet(Person) ah, size_t (*fhash)(Person), PredicateFunc(Person) fcond, Person (*copie)(Person))
{
    TAG(IEqualityComparer(Person)) myComparer = {
        .Equals = PersonEquals,
        .GetHashCode = fhash
    };
    HashSet(Person) result = new(HashSet(Person))(&myComparer);
    foreach (var person in ah) {
        if (fcond(person)) {
            HashSet_Person_Add(result, copie(person));
        }
    }
    return result;
}

#include <stdio.h>

static Person Person_copy_ctor(Person other)
{
    return (Person) {
        .Name = new(string)(other.Name),
        .PhoneNumber = other.PhoneNumber
    };
}

static bool ConditieNumar(Person pers)
{
    return (pers.PhoneNumber % 2) != 0;
}

int Main(void)
{
    var person_set = GenerareHash();
    Person items[] = {
        {"Andra",734769705},
        {"Andreea",734344283},
        {"Tibi",765726607},
        {"Matei",748889332},
        {"Joe Biden",777777777},
    };
    for (size_t i = 0; i < (sizeof(items) / sizeof(items[0])); ++i) {
        HashSet_Person_Add(person_set, items[i]);
    }
    foreach (var pers in person_set) {
        printf("Person: %s\nPhone number: %d\n", pers.Name, pers.PhoneNumber);
    }
    printf("Nume care incep cu \"An\": %d\n", NumaraPersoane(person_set, "An"));
    printf("Nou hash set din persoane al caror numar nu e div cu 2:\n");
    foreach (var pers in genereazaHashCond(person_set, PersonComparer->GetHashCode, ConditieNumar, Person_copy_ctor)) {
        printf("Person: %s\nPhone number: %d\n", pers.Name, pers.PhoneNumber);
    }
    return 0;
}
