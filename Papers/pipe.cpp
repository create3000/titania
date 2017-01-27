
static
std::string
golden_pipe (const std::string & program, const std::string & input)
//throw (Error <URL_UNAVAILABLE>)
{
	constexpr size_t BUFFER_SIZE = 1024;

	int32_t pid       = 0;
	int32_t status    = 0;
	int32_t stdin     = 0;
	int32_t stdout    = 0;
	int32_t stderr    = 0;
	size_t  bytesRead = 0;

	std::vector <char> buffer (BUFFER_SIZE);

	// Open pipe.

	if ((pid = os::popen3 (program, &stdin, &stdout, &stderr)) <= 0)
		throw Error <URL_UNAVAILABLE> ("Couldn't open program '" + program + "'.");

	// Write to pipe.

	if (write (stdin, input .c_str (), input .size ()) not_eq (int32_t) input .size ())
		throw Error <URL_UNAVAILABLE> ("Write to pipe failed.");

	close (stdin);

	// Read from pipe.

	std::string output;

	while ((bytesRead = read (stdout, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
		output .append (buffer .data (), bytesRead);

	close (stdout);

	// Read error from pipe.

	while ((bytesRead = read (stderr, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
		std::clog .write (buffer .data (), bytesRead);

	close (stderr);

	// Read from pipe.

	waitpid (pid, &status, 0);

	if (status)
		throw Error <INVALID_X3D> ("Exit status :" + basic::to_string (status, std::locale::classic ()));

	return output;
}

static
void
golden_x3d (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	static const std::string x3d2vrml = "titania-x3d2vrml";

	// Parse into stream.

	basic::ifilestream goldenstream (golden_pipe (x3d2vrml, basic::to_string (istream)));

	scene -> fromStream (uri, goldenstream);
	scene -> setEncoding (EncodingType::XML);
}
