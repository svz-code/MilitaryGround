import std.stdio;
import std.conv;
import std.string;
import std.algorithm;
import std.array;

void main() {
	int[4] arr = [2, 4, 7, 1];
	writeln("Привет, у нас есть вот такой массив: ", arr);
	writeln("Сумму с какого по какой элемент хочешь узнать?");
	writeln("Напоминаю, в массиве отсчет начинается с нуля :)");
	// В питоне и даже в Си было попроще :/
	int[] input = stdin.readln().strip().split().map!(to!int).array;
	
	int[] pref_arr = prefix(arr);
	writeln(input[0] == 0 ? pref_arr[input[1]] : pref_arr[input[1]] - pref_arr[input[0] - 1]);
}

int[] prefix(int[] arr) { 
	int[] pref_sums = new int[arr.length]; 
	pref_sums[0] = arr[0];
	
	for (int i = 1; i < arr.length; i++) { 
		pref_sums[i] = arr[i] + pref_sums[i-1];
	}
	
	return pref_sums;
} 
