#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
using namespace std::chrono_literals;
using namespace std::this_thread;

int main()
{
    init_apartment();

	// Declare variables
	HttpClient client; // Create an HttpClient to be used later
	hstring responseBody; // Declare publicly so 'try' AND 'catch' know the type
	hstring oldAmount; // Declare and initilize to null before comparing in if-else statement

	// Set up console to handle special characters and colors
	_setmode(_fileno(stdout), _O_U16TEXT); // Set the mode of the console output to Unicode UTF-16 to display special arrow characters
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // Set output mode to handle virtual terminal sequences
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	while (true) // Infinite loop
	{
		try
		{
			responseBody = client.GetStringAsync({ L"https://api.coinbase.com/v2/prices/spot?currency=USD" }).get(); // Use HttpClient to grab JSON string async and store it in responseBody
			JsonObject jsonObject = JsonObject::Parse(responseBody); // Parse the JSON string and store it as a JsonObject type
			IJsonValue dataValue = jsonObject.Lookup(L"data"); // Store values after the 'data' key as IJsonValue typee
			hstring s = dataValue.Stringify(); // Must convert IJSonValue type back to a JSON string to parse again
			JsonObject dataObject = JsonObject::Parse(s); // Parse the new JSON string with the 'data' key stripped off and store it as a JsonObject type
			hstring newAmount = dataObject.GetNamedString(L"amount"); // Store value of 'amount' key in string as the new amount
			if (newAmount >= oldAmount) // If the new amount is greater than or euqal to the old amount...
			{
				std::wcout << L"BTCUSD ↑ $\x1b[92m" << newAmount.c_str() << L"\x1b[0m\n"; // Then output to console with an up arrow, color it bright green, convert hstring to wchar_t, then reset the color back to default
			}
			else
			{
				std::wcout << L"BTCUSD ↓ $\x1b[91m" << newAmount.c_str() << L"\x1b[0m\n"; // Else output to console with a down arrow and color it red
			}
			oldAmount = newAmount; // Store the current amount as the old amount so we can compare it in if-else statement after next run
			sleep_for(31s); // Sleep for 31 seconds to reduce duplicate amounts from 30 second cycle on coinbase server side
		}
		catch (hresult_error const& ex) // Try to catch errors from HttpClient
		{
			responseBody = ex.message(); // Possible simplification and eliminate public declaration of 'responseBody' above, prints to console?
		}
	}
}
