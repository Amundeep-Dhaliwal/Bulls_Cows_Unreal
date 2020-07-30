// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h" 


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    
    //PrintLine(TEXT("Howdy there partner! Mighty fine day\nwe are experiencing!"));
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);

    Isograms = GetValidWords(WordList);
    SetupGame(Isograms);


    //PrintLine(TEXT("The number of valid words is %i"), Words.Num());
    
    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{

    //pre-increment(++c) returns incremented value whereas post-increment(c++) returns the original value 

    if(bGameOver){
        ClearScreen();
        SetupGame(Isograms);
        }
    else{
        ProcessGuess(PlayerInput);
        }
}

void UBullCowCartridge::SetupGame(const TArray<FString>& Choose){
    
    
    HiddenWord = Choose[FMath::RandRange(0,Choose.Num()-1)];
    Lives = HiddenWord.Len(); // variable initialization
    bGameOver = false;
    
    int Player_attempts = 0;
    
    PrintLine(TEXT("Please guess the %i letter word!"), HiddenWord.Len()); 
    PrintLine(TEXT("You may now enter whatever you like!\nKeep in mind that you have %i lives.\nDon't forget to press enter!"), Lives);
    PrintLine(TEXT("The word is %s and it is %i characters\nlong"), *HiddenWord, HiddenWord.Len());

    
}

void UBullCowCartridge::EndGame(){
    
    bGameOver = true; // this needs to be fixed 
    Player_attempts = 0;
    PrintLine(TEXT("\nPress enter to play again"));
    
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    Player_attempts++;
    if(Guess.ToLower() == HiddenWord.ToLower()){
        PrintLine(TEXT("You have correctly guessed %s\nin %i attempts!"), *HiddenWord, Player_attempts);
        EndGame();
        return;
        }
    if(Guess.Len() != HiddenWord.Len()){
        PrintLine(TEXT("Sorry try guessing again!\nThe hidden word is %i characters!\nYou have %i lives remaining..."), HiddenWord.Len(),--Lives);
        return;
        }
    PrintLine(TEXT("You have lost a life!"));
    --Lives;
    if(Lives < 1){
        ClearScreen();
        PrintLine(TEXT("You have lost all your lives!"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
        return;
        }
    if(!IsIsogram(Guess)){
        PrintLine(TEXT("The word has no repeating letters!"));
    }
    
    // Show Bulls and Cows 
    //int32 Bulls, Cows; //Uninitialized parameters as parameters in function call
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i bulls and %i cows!"), Score.Bulls,Score .Cows);

    PrintLine(TEXT("I invite you to guess again! You have %i\nlives remaining..."), Lives);
    
}



TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Potential) const
{
    TArray<FString> ValidWords;

    for(FString Word: Potential){
            if(IsIsogram(Word)){
                ValidWords.Emplace(Word);
            }
            //PrintLine(TEXT("%s"), *WordList[i]);
        }
    // for(int32 j = 0;j < ValidWords.Num(); j++){
    //     PrintLine(TEXT("%s"), *ValidWords[j]);
    // }
    //PrintLine(TEXT("The number of potential words is %i"), Potential.Num());
    PrintLine(TEXT("The number of valid words is %i"), ValidWords.Num());
    return ValidWords;
}
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    for(int32 i =0; i<Word.Len(); i++){
        for(int32 j = i+1; j < Word.Len(); j++){
            if((Word[i] == Word[j]) && (j != i)){
                return false;
            }
        }
    }
    
    return true;
}

 FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for(int32 i = 0; i < Guess.Len(); i++){
        if(Guess[i] == HiddenWord[i]){
            Count.Bulls++;
            continue;
        }
        for(int32 j = 0; j < HiddenWord.Len(); j++){
            if(Guess[i] == HiddenWord[j]){
                Count.Cows++; 
                break; // Prevents wasted iteration cycles
            }
        }   
    }
    return Count;
}