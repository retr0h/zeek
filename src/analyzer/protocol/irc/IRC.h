// An IRC analyzer contributed by Roland Gruber.

#pragma once

#include "zeek/analyzer/protocol/tcp/TCP.h"
#include "zeek/analyzer/protocol/tcp/ContentLine.h"

namespace zeek::analyzer::irc {

/**
* \brief Main class for analyzing IRC traffic.
*/
class IRC_Analyzer final : public analyzer::tcp::TCP_ApplicationAnalyzer {
	enum { WAIT_FOR_REGISTRATION, REGISTERED, };
	enum { NO_ZIP, ACCEPT_ZIP, ZIP_LOADED, };
public:
	/**
	* \brief Constructor, builds a new analyzer object.
	*/
	explicit IRC_Analyzer(Connection* conn);

	/**
	* \brief Called when connection is closed.
	*/
	void Done() override;

	/**
	* \brief New input line in network stream.
	*
	* \param len the line length
	* \param data pointer to line start
	* \param orig was this data sent from connection originator?
	*/
	void DeliverStream(int len, const u_char* data, bool orig) override;

	static analyzer::Analyzer* Instantiate(Connection* conn)
		{
		return new IRC_Analyzer(conn);
		}

protected:
	int orig_status;
	int orig_zip_status;
	int resp_status;
	int resp_zip_status;

private:
	void StartTLS();

	inline void SkipLeadingWhitespace(std::string& str);

	/** \brief counts number of invalid IRC messages */
	int invalid_msg_count;

	/** \brief maximum count of invalid IRC messages */
	int invalid_msg_max_count;

	/**
	* \brief Splits a string into its words which are separated by
	* the split character.
	*
	* \param input string which will be splitted
	* \param split character which separates the words
	* \return vector containing words
	*/
	std::vector<std::string> SplitWords(const std::string& input, char split);

	analyzer::tcp::ContentLine_Analyzer* cl_orig;
	analyzer::tcp::ContentLine_Analyzer* cl_resp;
	bool starttls; // if true, connection has been upgraded to tls
};

} // namespace zeek::analyzer::irc
