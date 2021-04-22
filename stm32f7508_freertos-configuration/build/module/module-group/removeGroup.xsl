<?xml version="1.0" encoding="UTF-8"?>

<!--
	XSL
	
	Copyright 2015-2020 MicroEJ Corp. All rights reserved.
	This library is provided in source code for use, modification and test, subject to license terms.
	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
	
	Bibliography:
		[XSLTREC] XSL Transformations (XSLT), Version 1.0, W3C Recommendation 16 November 1999
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- This is an XML to XML generator -->
	<xsl:output	method="xml" indent="yes"/>

	<!-- Well known identity transformation : see 7.5 in [XSLTREC] -->
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="group[@name='%GROUP_NAME%']"/>
</xsl:stylesheet>